#pragma once

#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

//adc sample channel and filter
#define ADC_CHANNEL_NUM   12
#define SAMPLE_BUFFER_NUM 40
#define TRIM_NUMBER       15
#define NTC_TABLE_SIZE    176

static const int16_t TEMP_MIN_10X = -500;
static const int16_t TEMP_MAX_10x = 1250;

static const int16_t TEMP_SENSOR_FAILURE_VALUE = INT16_MIN;


// NTC ADC-T Table From deepseek convert the R-T table
// R_fixed = 10kΩ，ADC 12位 (0~4095)，B=3380，25℃=10kΩ
// range：-50℃ ~ 125℃
static const uint16_t ntc_rt_table_3380[NTC_TABLE_SIZE] = 
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

typedef enum 
{
    ADC_IDX_PRESS_INTAKE    = 0,   // CH0  PA0  
    ADC_IDX_CURRENT_2       = 1,   // CH1  PA1  
    ADC_IDX_CURRENT_1       = 2,   // CH2  PA2  
    ADC_IDX_CURRENT_3       = 3,   // CH3  PA3  
    ADC_IDX_TEMP_STORAGE    = 4,   // CH5  PA5  
    ADC_IDX_TEMP_DEF        = 5,   // CH6  PA6  
    ADC_IDX_TEMP_INTAKE     = 6,   // CH7  PA7  
    ADC_IDX_TEMP_EXHAUST    = 7,   // CH9  PB1  
    ADC_IDX_TEMP_CONDEN     = 8,   // CH10 PB2  
    ADC_IDX_TEMP_AMBIENT    = 9,   // CH11 PB10 
    ADC_IDX_TEMP_SPRAY      = 10,  // CH15 PB11 
    ADC_IDX_PRESS_EXHAUST   = 11,  // CH16 PB12 
    ADC_IDX_MAX             = 12
} ADC_Index_E;

typedef struct 
{
    //press
    uint16_t press_intake[SAMPLE_BUFFER_NUM];
    uint16_t press_exhaust[SAMPLE_BUFFER_NUM];

    //current
    uint16_t current_1[SAMPLE_BUFFER_NUM];
    uint16_t current_2[SAMPLE_BUFFER_NUM];
    uint16_t current_3[SAMPLE_BUFFER_NUM];

    //temperature
    uint16_t temp_storage[SAMPLE_BUFFER_NUM];
    uint16_t temp_defrost[SAMPLE_BUFFER_NUM];
    uint16_t temp_intake[SAMPLE_BUFFER_NUM];
    uint16_t temp_exhaust[SAMPLE_BUFFER_NUM];
    uint16_t temp_condensing[SAMPLE_BUFFER_NUM];
    uint16_t temp_ambient[SAMPLE_BUFFER_NUM];
    uint16_t temp_spray[SAMPLE_BUFFER_NUM];
    
    uint16_t sample_count;
}SAMPLE_BUFFER_ST;

typedef struct
{
    uint16_t adc_buffer[ADC_CHANNEL_NUM];
    uint16_t adc_processed_data_10x[ADC_CHANNEL_NUM];
}ADC_DATA_BUFFER_ST;

void adc_task(void* pvParameters);

extern TaskHandle_t adc_task_handle;
