#include "adc_app.h"
#include "adc.h"
#include "portmacro.h"
#include "projdefs.h"
#include "stm32g0xx_hal_adc.h"

TaskHandle_t adc_task_handle = NULL;

uint16_t g_adc_buffer[ADC_CHANNEL_NUM];

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    (void)hadc;
    BaseType_t woken = pdFALSE;
    vTaskNotifyGiveFromISR(adc_task_handle, &woken);
    portYIELD_FROM_ISR(woken);
}

void adc_task(void* pvParameters)
{
    (void) pvParameters;
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)g_adc_buffer, ADC_CHANNEL_NUM);

    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}
