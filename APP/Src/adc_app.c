#include "adc_app.h"
#include "adc.h"
#include "portmacro.h"
#include "projdefs.h"
#include "stm32g0xx_hal_adc.h"
#include "stdbool.h"
#include "adc_filter.h"
#include <stdint.h>
#include <stdio.h>
#include "log.h"

TaskHandle_t adc_task_handle = NULL;

static SAMPLE_BUFFER_ST sample_buffer_st;
static ADC_DATA_BUFFER_ST adc_data_st;

static const int16_t TEMP_MIN_10X = -500;
static const int16_t ADC_NULL = 0;

// NTC ADC-T Table From deepseek convert the R-T table
// R_fixed = 10kΩ，ADC 12位 (0~4095)，25℃=10kΩ
// range：-50℃ ~ 125℃
// ADC = (R_ntc / (R_ntc + R_fixed)) * ADC_max, R_ntc in R-T table, R_fixed = 10k
// ADC_MAX = 2^12 = 4096

//B value = 3435，
static const uint16_t adc_temp_3435[NTC_TABLE_SIZE] = 
{
    3974, 3968, 3960, 3953, 3945, 3936, 3928, 3919, 3909, 3899,  // -50 ~ -41
    3889, 3878, 3867, 3855, 3842, 3830, 3816, 3802, 3788, 3773,  // -40 ~ -31
    3758, 3741, 3725, 3708, 3690, 3671, 3652, 3632, 3612, 3591,  // -30 ~ -21
    3569, 3546, 3523, 3500, 3475, 3450, 3424, 3398, 3372, 3343,  // -20 ~ -11
    3315, 3286, 3256, 3226, 3195, 3163, 3131, 3099, 3066, 3032,  // -10 ~ -1
    2997, 2963, 2927, 2892, 2856, 2819, 2783, 2745, 2707, 2669,  // 0 ~ 9
    2631, 2593, 2555, 2516, 2477, 2438, 2399, 2360, 2321, 2281,  // 10 ~ 19
    2242, 2203, 2164, 2126, 2087, 2048, 2011, 1972, 1935, 1897,  // 20 ~ 29
    1860, 1823, 1786, 1750, 1714, 1679, 1644, 1609, 1575, 1542,  // 30 ~ 39
    1508, 1476, 1443, 1411, 1380, 1350, 1320, 1290, 1261, 1232,  // 40 ~ 49
    1204, 1176, 1149, 1122, 1096, 1070, 1045, 1021,  997,  974,  // 50 ~ 59
     951,  928,  906,  884,  863,  843,  822,  803,  783,  765,  // 60 ~ 69
     746,  728,  711,  694,  677,  661,  645,  630,  615,  600,  // 70 ~ 79
     586,  572,  558,  545,  532,  519,  507,  495,  483,  471,  // 80 ~ 89
     460,  450,  439,  429,  419,  409,  399,  390,  381,  372,  // 90 ~ 99
     363,  355,  347,  339,  331,  324,  316,  309,  302,  295,  // 100 ~ 109
     288,  282,  276,  270,  264,  258,  252,  246,  241,  236,  // 110 ~ 119
     231,  226,  221,  216,  211,  207                                    // 120 ~ 125
};

//B value = 3380，
static const uint16_t adc_temp_3380[NTC_TABLE_SIZE] = 
{
    3987, 3980, 3973, 3965, 3956, 3949, 3939, 3931, // -50℃ ~ -43℃
    3922, 3912, 3901, 3891, 3879, 3868, 3855, 3843, // -42℃ ~ -35℃
    3829, 3815, 3801, 3786, 3771, 3755, 3738, 3720, // -34℃ ~ -27℃
    3702, 3684, 3665, 3644, 3624, 3603, 3581, 3558, // -26℃ ~ -19℃
    3535, 3511, 3486, 3461, 3435, 3408, 3380, 3352, // -18℃ ~ -11℃
    3324, 3294, 3264, 3234, 3202, 3170, 3138, 3105, // -10℃ ~  -3℃
    3071, 3037, 3002, 2967, 2932, 2896, 2859, 2822, //  -2℃ ~   5℃
    2785, 2747, 2709, 2671, 2633, 2594, 2555, 2516, //   6℃ ~  13℃
    2477, 2438, 2399, 2359, 2320, 2281, 2242, 2202, //  14℃ ~  21℃
    2163, 2124, 2086, 2048, 2009, 1971, 1933, 1895, //  22℃ ~  29℃
    1858, 1821, 1785, 1749, 1713, 1677, 1642, 1608, //  30℃ ~  37℃
    1574, 1540, 1507, 1474, 1442, 1410, 1379, 1348, //  38℃ ~  45℃
    1318, 1288, 1259, 1231, 1203, 1175, 1148, 1122, //  46℃ ~  53℃
    1095, 1070, 1045, 1021,  997,  973,  950,  928, //  54℃ ~  61℃
     906,  884,  863,  843,  823,  803,  784,  765, //  62℃ ~  69℃
     747,  729,  711,  695,  678,  662,  646,  630, //  70℃ ~  77℃
     615,  601,  586,  572,  559,  545,  532,  520, //  78℃ ~  85℃
     508,  496,  484,  472,  461,  450,  440,  429, //  86℃ ~  93℃
     419,  410,  400,  391,  382,  373,  364,  356, //  94℃ ~ 101℃
     348,  340,  332,  324,  317,  310,  303,  296, // 102℃ ~ 109℃
     289,  283,  276,  270,  264,  258,  252,  247, // 110℃ ~ 117℃
     241,  236,  231,  226,  222,  217,  212,  207  // 118℃ ~ 125℃
};

//B value = 3950
static const uint16_t adc_temp_3950[NTC_TABLE_SIZE] = 
{
    4039, 4035, 4031, 4026, 4021, 4015, 4009, 4003, 3997, 3990,  // -50 ~ -41
    3983, 3975, 3967, 3958, 3949, 3940, 3929, 3919, 3907, 3896,  // -40 ~ -31
    3883, 3870, 3856, 3842, 3827, 3811, 3794, 3777, 3759, 3740,  // -30 ~ -21
    3720, 3700, 3679, 3656, 3633, 3609, 3585, 3559, 3532, 3505,  // -20 ~ -11
    3476, 3447, 3416, 3385, 3353, 3320, 3286, 3251, 3215, 3179,  // -10 ~ -1
    3142, 3103, 3065, 3025, 2985, 2944, 2902, 2860, 2817, 2773,  // 0 ~ 9
    2730, 2686, 2641, 2596, 2551, 2505, 2460, 2413, 2368, 2322,  // 10 ~ 19
    2276, 2230, 2184, 2138, 2093, 2048, 2003, 1958, 1914, 1870,  // 20 ~ 29
    1826, 1783, 1741, 1698, 1657, 1616, 1576, 1536, 1497, 1459,  // 30 ~ 39
    1421, 1384, 1347, 1312, 1277, 1242, 1209, 1176, 1144, 1112,  // 40 ~ 49
    1082, 1052, 1022,  994,  966,  939,  912,  886,  861,  837,  // 50 ~ 59
     813,  790,  767,  745,  724,  703,  683,  663,  644,  626,  // 60 ~ 69
     608,  590,  573,  557,  541,  525,  510,  496,  481,  468,  // 70 ~ 79
     454,  441,  429,  417,  405,  394,  382,  372,  362,  351,  // 80 ~ 89
     341,  332,  323,  314,  305,  297,  289,  280,  273,  265,  // 90 ~ 99
     258,  251,  245,  238,  231,  225,  219,  214,  208,  202,  // 100 ~ 109
     197,  192,  187,  182,  177,  173,  168,  164,  159,  156,  // 110 ~ 119
     151,  148,  144,  140,  137,  134                                    // 120 ~ 125
};


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
    sample_buffer_st.temp_exhaust[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_EXHAUST];
    sample_buffer_st.temp_condensing[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_CONDEN];
    sample_buffer_st.temp_ambient[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_AMBIENT];
    sample_buffer_st.temp_spray[sample_buffer_st.sample_count] = adc_data_st.adc_buffer[ADC_IDX_TEMP_SPRAY];

    sample_buffer_st.sample_count++;

    return false;
}

//get the filter origin data
static void adc_filter_task(void)
{
    //sample task is ready, sort and get the median number five times and do average calculation
    adc_data_st.adc_buffer[ADC_IDX_PRESS_INTAKE] = mean_middle(sample_buffer_st.press_intake, SAMPLE_BUFFER_NUM, TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_PRESS_EXHAUST] = mean_middle(sample_buffer_st.press_exhaust,SAMPLE_BUFFER_NUM, TRIM_NUMBER);

    adc_data_st.adc_buffer[ADC_IDX_CURRENT_1] = mean_topk(sample_buffer_st.current_1,SAMPLE_BUFFER_NUM, TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_CURRENT_2] = mean_topk(sample_buffer_st.current_2,SAMPLE_BUFFER_NUM, TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_CURRENT_3] = mean_topk(sample_buffer_st.current_3,SAMPLE_BUFFER_NUM, TRIM_NUMBER);

    adc_data_st.adc_buffer[ADC_IDX_TEMP_STORAGE] = mean_middle(sample_buffer_st.temp_storage,SAMPLE_BUFFER_NUM, TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_DEF] = mean_middle(sample_buffer_st.temp_defrost,SAMPLE_BUFFER_NUM, TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_INTAKE] = mean_middle(sample_buffer_st.temp_intake,SAMPLE_BUFFER_NUM, TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_EXHAUST] = mean_middle(sample_buffer_st.temp_exhaust,SAMPLE_BUFFER_NUM, TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_CONDEN] = mean_middle(sample_buffer_st.temp_condensing,SAMPLE_BUFFER_NUM, TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_AMBIENT] = mean_middle(sample_buffer_st.temp_ambient,SAMPLE_BUFFER_NUM, TRIM_NUMBER);
    adc_data_st.adc_buffer[ADC_IDX_TEMP_SPRAY] = mean_middle(sample_buffer_st.temp_spray,SAMPLE_BUFFER_NUM, TRIM_NUMBER);
}

static void adc_sensor_error_handler(uint16_t adc_index)
{
    uint16_t num_rstc = 32;
    char err_desc[num_rstc];
    sniprintf(err_desc, num_rstc, "SENSOR FLASE :");

    switch (adc_index) 
    {
        case ADC_IDX_PRESS_INTAKE:
            sniprintf(err_desc, num_rstc, " P-INTAKE");
            break;

        case ADC_IDX_CURRENT_2:
            sniprintf(err_desc, num_rstc, " CURRENT 2");
            break;

        case ADC_IDX_CURRENT_1:
            sniprintf(err_desc, num_rstc, " CURRENT 1");
            break;

        case ADC_IDX_CURRENT_3:
            sniprintf(err_desc, num_rstc, " CURRENT 3");
            break;

        case ADC_IDX_TEMP_STORAGE:
            sniprintf(err_desc, num_rstc, " STORAGE");
            break;

        case ADC_IDX_TEMP_DEF:
            sniprintf(err_desc, num_rstc, " DEF");
            break;

        case ADC_IDX_TEMP_INTAKE:
            sniprintf(err_desc, num_rstc, " T-INTAKE");
            break;

        case ADC_IDX_TEMP_EXHAUST:
            sniprintf(err_desc, num_rstc, " T-EXHAUST");
            break;

        case ADC_IDX_TEMP_CONDEN:
            sniprintf(err_desc, num_rstc, " CONDEN");
            break;

        case ADC_IDX_TEMP_AMBIENT:
            sniprintf(err_desc, num_rstc, " AMBIENT");
            break;

        case ADC_IDX_TEMP_SPRAY:
            sniprintf(err_desc, num_rstc, " SPRAY");
            break;

        case ADC_IDX_PRESS_EXHAUST:
            sniprintf(err_desc, num_rstc, " P-EXHAUST");
            break;
    }

    LOG_ERROR("%s",err_desc);
}

//convertion: Steinhart-Hart Formula, 1/T = A + B * ln(R) + C * (ln(R))^3
//B-value bigger means NTC is sensitive to temperature change
//1. if do not have ntc manual, get A,B,C by 3 groups equations
//2. if you have R-T table, better. you can get ADC-T table by R-T table through 
static int16_t scale_transformation_temperature_10x(uint16_t adc_index, uint8_t b_value)
{
    uint16_t adc_temp = adc_data_st.adc_buffer[adc_index];
    const uint16_t* adc_temp_table = {0}; 

    if(b_value == B_VALUE_3380)
    {
        adc_temp_table = adc_temp_3380;
    }
    else if(b_value == B_VALUE_3435)
    {
        adc_temp_table = adc_temp_3435;
    }
    else if(b_value == B_VALUE_3950)
    {
        adc_temp_table = adc_temp_3950;
    }
    else 
    {
        return 0;
    }

    //boarder check by ADC-T table
    bool beyond_upper_limit = (adc_temp > adc_temp_table[0]);
    bool beyond_lower_limit = (adc_temp < adc_temp_table[NTC_TABLE_SIZE - 1]);
    if(beyond_lower_limit || beyond_upper_limit)
    {
        adc_sensor_error_handler(adc_index);
        return ADC_NULL;
    }

    //binary search
    uint16_t left = 0;
    uint16_t right = NTC_TABLE_SIZE - 1;
    while(left + 1 < right)
    {
        uint16_t middle = left + (right - left) / 2;
        if (adc_temp > adc_temp_table[middle]) 
            right = middle;
        else 
            left = middle; 
    }
    
    int16_t temp_base = TEMP_MIN_10X + (int16_t)left * 10;
    int16_t adc_delta = adc_temp - adc_temp_table[left];
    int16_t adc_range = adc_temp_table[right] - adc_temp_table[left];

    int16_t temp = temp_base + ((adc_delta * 10) / adc_range);
    return temp;
}

// specification and module : Low (0.5-4.5dc | 0-2Mpa), High (0.5-4.5dc | 0-4.6Mpa)
// Vref = 5V
// Vadc = adc / 4096 * Vref
// Vmin, Vrange, Prange is in Pressure sensor data sheet
// P = (Vadc - Vmin) / Vrange * Prange
static uint16_t scale_transformation_pressure_100x(uint16_t adc_index)
{
    uint16_t adc_press = adc_data_st.adc_buffer[adc_index];
    //100x
    uint16_t adc_v = (adc_press * 100) / 4096 * 5;
    
    uint8_t beyond_lower_limit = (adc_v < 50);
    uint8_t beyond_upper_limit = (adc_v > 450);
    if(beyond_lower_limit || beyond_upper_limit)
    {
        adc_sensor_error_handler(adc_index);
        return ADC_NULL;
    }
    
    // 4.5 - 0.5 = 4
    uint16_t dc_range = 4;
    // 1bar = 0.1Mpa
    uint16_t press_range = 0;

    if(adc_index == ADC_IDX_PRESS_EXHAUST)
    {
        press_range = 46;
    }
    else if(adc_index == ADC_IDX_PRESS_INTAKE)
    {
        press_range = 20;
    }

    uint16_t press = (adc_v - 50) / dc_range * press_range;
    return press;
}

// specification and module : ZHT102W(5A/2.5mA) ZHT350C(5A/2.5mA)
// N: 2000:1, Vref = 3.3v, R(sample) = 100Ω， 0.7V is voltage decrease by diode，is physical propety
// V(adc_peak) = adc_max / 4096 * Vref
// V(sample-r-peak) = V(adc_peak) + 0.7V
// I = V(sample-r-peak) / R(sample)
// Irms = I * 1/√2
static uint16_t scale_transformation_current_100x(uint16_t adc_index)
{
    uint16_t adc_max = adc_data_st.adc_buffer[adc_index];
    uint16_t N = 2000;

    //330 = Vref * 100;
    uint16_t adc_peak = adc_max / 4096 * 330;
    //70 = 0.7 * 100
    uint16_t phy_real_peak = adc_peak + 70;
    uint16_t cur_peak = phy_real_peak / 100;
    uint16_t rms = cur_peak * 1000 / 1414 * N;

    return rms;
}

static void adc_threshold_alarm_handler(void)
{
    
}

void adc_task(void* pvParameters)
{
    (void) pvParameters;
    UBaseType_t uxHighWaterMark;
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

        //finish sampling
        if(adc_sample_task())
        {
            //filter
            adc_filter_task();
            
            //scale transformation: origin sample data -> prorcessed
            
            //temperature
            adc_data_st.adc_processed_data[ADC_IDX_TEMP_STORAGE] = scale_transformation_temperature_10x(ADC_IDX_TEMP_STORAGE, B_VALUE_3380);
            adc_data_st.adc_processed_data[ADC_IDX_TEMP_DEF] = scale_transformation_temperature_10x(ADC_IDX_TEMP_DEF, B_VALUE_3380);
            adc_data_st.adc_processed_data[ADC_IDX_TEMP_INTAKE] = scale_transformation_temperature_10x(ADC_IDX_TEMP_INTAKE, B_VALUE_3435);
            adc_data_st.adc_processed_data[ADC_IDX_TEMP_EXHAUST] = scale_transformation_temperature_10x(ADC_IDX_TEMP_EXHAUST, B_VALUE_3435);
            adc_data_st.adc_processed_data[ADC_IDX_TEMP_CONDEN] = scale_transformation_temperature_10x(ADC_IDX_TEMP_CONDEN, B_VALUE_3950);
            adc_data_st.adc_processed_data[ADC_IDX_TEMP_AMBIENT] = scale_transformation_temperature_10x(ADC_IDX_TEMP_AMBIENT, B_VALUE_3950);
            adc_data_st.adc_processed_data[ADC_IDX_TEMP_SPRAY] = scale_transformation_temperature_10x(ADC_IDX_TEMP_SPRAY, B_VALUE_3435);

            //pressure
            adc_data_st.adc_processed_data[ADC_IDX_PRESS_INTAKE] = scale_transformation_pressure_100x(ADC_IDX_PRESS_INTAKE);
            adc_data_st.adc_processed_data[ADC_IDX_PRESS_EXHAUST] = scale_transformation_pressure_100x(ADC_IDX_PRESS_EXHAUST);

            //current
            adc_data_st.adc_processed_data[ADC_IDX_CURRENT_1] = scale_transformation_current_100x(ADC_IDX_CURRENT_1);
            adc_data_st.adc_processed_data[ADC_IDX_CURRENT_2] = scale_transformation_current_100x(ADC_IDX_CURRENT_2);
            adc_data_st.adc_processed_data[ADC_IDX_CURRENT_3] = scale_transformation_current_100x(ADC_IDX_CURRENT_3);

            //print all temperature sensor value
            LOG_DEBUG("TEMP STORAGE =%d, DEF =%d, INTAKE =%d, EXHAUST =%d, CONDEN =%d, AMBIENT =%d, SPARY =%d PRESS INTAKE =%d, EXHAUST =%d CURRENT C1 =%d, C2 =%d, C3 =%d",
                    adc_data_st.adc_processed_data[ADC_IDX_TEMP_STORAGE], adc_data_st.adc_processed_data[ADC_IDX_TEMP_DEF],
                    adc_data_st.adc_processed_data[ADC_IDX_TEMP_INTAKE], adc_data_st.adc_processed_data[ADC_IDX_TEMP_EXHAUST],
                    adc_data_st.adc_processed_data[ADC_IDX_TEMP_CONDEN], adc_data_st.adc_processed_data[ADC_IDX_TEMP_AMBIENT],
                    adc_data_st.adc_processed_data[ADC_IDX_TEMP_SPRAY],
                    adc_data_st.adc_processed_data[ADC_IDX_PRESS_INTAKE], adc_data_st.adc_processed_data[ADC_IDX_PRESS_EXHAUST],
                    adc_data_st.adc_processed_data[ADC_IDX_CURRENT_1], adc_data_st.adc_processed_data[ADC_IDX_CURRENT_2],
                    adc_data_st.adc_processed_data[ADC_IDX_CURRENT_3]);

            //test the deepest stack used condition
            uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
            LOG_INFO("the deepeset is %lu", uxHighWaterMark);
        }
    }
}
