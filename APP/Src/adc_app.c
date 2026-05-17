#include "adc_app.h"
#include "adc.h"
#include "portmacro.h"
#include "projdefs.h"
#include "stm32g0xx_hal_adc.h"
#include "stdbool.h"

TaskHandle_t adc_task_handle = NULL;

static SAMPLE_BUFFER_ST sample_buffer_st;
static ADC_DATA_BUFFER_ST adc_data_st;

//DMA Normal Mode
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    (void) hadc;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //if adcTaskHandle has high priority, xHigherPriorityTaskWoken is true
    vTaskNotifyGiveFromISR(adc_task_handle, &xHigherPriorityTaskWoken);
    //let scheduler switch to high priority task
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

//sample in a circular buffer,data won't be until the sample is finished
static bool adc_sample_task(void)
{
    if(sample_buffer_st.sample_count >= SAMPLE_BUFFER_NUM)
    {
        sample_buffer_st.sample_count = 0;
        return true;
    }

    //pushed into sample buffer and wait to filtered
    //press
    sample_buffer_st.press_intake[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_PRESS_INTAKE];
    sample_buffer_st.press_exhaust[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_PRESS_EXHAUST];
    //current
    sample_buffer_st.current_1[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_CURRENT_1];
    sample_buffer_st.current_2[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_CURRENT_2];
    sample_buffer_st.current_3[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_CURRENT_3];
    //temperature
    sample_buffer_st.temp_storage[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_STORAGE];
    sample_buffer_st.temp_defrost[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_DEF];
    sample_buffer_st.temp_intake[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_INTAKE];
    sample_buffer_st.temp_exhaust[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_PRESS_EXHAUST];
    sample_buffer_st.temp_condensing[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_CONDEN];
    sample_buffer_st.temp_ambient[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_AMBIENT];
    sample_buffer_st.temp_spray[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_SPRAY];

    sample_buffer_st.sample_count++;

    return false;
}

//temperature and pressure type data filter algrithom
static uint16_t sort_and_median_average(uint16_t* buffer, uint16_t trim_num)
{
    //* insert sort
    //algrithmn understanding: image that a row of students, start from the first one, ask another student "are you older than me" who in front of this
    //if yes the asked students move back one step, if not, the asking student sit at the current position    
    for(int i = 1; i < SAMPLE_BUFFER_NUM; i++)
    {
        uint16_t key = buffer[i];
        uint16_t j = i - 1;
        while(j >= 0 && buffer[j] > key)
        {
            buffer[j + 1] = buffer[j];
            j--;
        }
        buffer[j + 1] = key;
    }

    //trim at both sides and get the average
    uint32_t sum = 0;
    uint16_t average;
    for(int i = trim_num; i < SAMPLE_BUFFER_NUM - trim_num; i++)
    {
        sum += buffer[i];
    }

    average = sum / (SAMPLE_BUFFER_NUM - trim_num * 2);
    return average;
}

//current type data algrithom
static uint16_t sort_and_topk_average(uint16_t* buffer, uint16_t trim_num)
{
    //* insert sort
    //algrithmn understanding: image that a row of students, start from the first one, ask another student "are you older than me" who in front of this
    //if yes the asked students move back one step, if not, the asking student sit at the current position    
    for(int i = 1; i < SAMPLE_BUFFER_NUM; i++)
    {
        uint16_t key = buffer[i];
        uint16_t j = i - 1;
        while(j >= 0 && buffer[j] > key)
        {
            buffer[j + 1] = buffer[j];
            j--;
        }
        buffer[j + 1] = key;
    }

    //trim at both sides and get the average
    uint32_t sum = 0;
    uint16_t average;
    for(int i = trim_num * 2; i < SAMPLE_BUFFER_NUM; i++)
    {
        sum += buffer[i];
    }

    average = sum / (SAMPLE_BUFFER_NUM - trim_num * 2);
    return average;
}

//get the filter origin data
static void adc_filter_task(void)
{
    //sample task is ready, sort and get the median number five times and do average calculation
    adc_data_st.adc_buffer[ADC_IDX_PRESS_INTAKE] = sort_and_median_average(sample_buffer_st.press_intake,TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_PRESS_EXHAUST] = sort_and_median_average(sample_buffer_st.press_exhaust,TRIM_NUMBER);

    adc_data_st.adc_buffer[ADC_IDX_CURRENT_1] = sort_and_topk_average(sample_buffer_st.current_1,TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_CURRENT_2] = sort_and_topk_average(sample_buffer_st.current_2,TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_CURRENT_3] = sort_and_topk_average(sample_buffer_st.current_3,TRIM_NUMBER);

    adc_data_st.adc_buffer[ADC_IDX_TEMP_STORAGE] = sort_and_median_average(sample_buffer_st.temp_storage,TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_DEF] = sort_and_median_average(sample_buffer_st.temp_defrost,TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_INTAKE] = sort_and_median_average(sample_buffer_st.temp_intake,TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_EXHAUST] = sort_and_median_average(sample_buffer_st.temp_exhaust,TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_CONDEN] = sort_and_median_average(sample_buffer_st.temp_condensing,TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_AMBIENT] = sort_and_median_average(sample_buffer_st.temp_ambient,TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_SPRAY] = sort_and_median_average(sample_buffer_st.temp_spray,TRIM_NUMBER);
}

//convertion: Steinhart-Hart Formula, 1/T = A + B * ln(R) + C * (ln(R))^3
//B-value bigger means NTC is sensitive to temperature change
//1. if do not have ntc manual, get A,B,C by 3 groups equations
//2. if you have R-T table, better. you can get ADC-T table by R-T table through 
//   ADC = (R_ntc / (R_ntc + R_fixed)) * ADC_max, R_ntc in R-T table, R_fixed = 10k
//   ADC_MAX = 2^12 = 4096
static void scale_transformation_temperture()
{
    

}

static void scale_transformation_pressure()
{

}

static void scale_transformation_current()
{

}

void adc_task(void* pvParameters)
{
    (void) pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(1);  // 1ms
    for(;;)
    {
        //1ms cycle
        xTaskDelayUntil(&xLastWakeTime, xPeriod);
        //restart
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_data_st.adc_buffer, ADC_CHANNEL_NUM);
        //if not take the sigle, here will be suspended
        //if take the sigle, pdTRUE -> pdFALSE
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if(adc_sample_task())
        {
            //filter
            adc_filter_task();
            
            //scale transformation: origin sample data -> human readable
            scale_transformation_temperture();
        }
        
    }
}
