#pragma once

#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

//adc sample channel and filter
#define ADC_CHANNEL_NUM   12
#define TRIM_NUMBER       15
#define SAMPLE_BUFFER_NUM 40  // SAMPLE_BUFFER_NUM > TRIM_NUMBER * 2
#define NTC_TABLE_SIZE    176

typedef enum
{
    B_VALUE_3380 = 0,
    B_VALUE_3435 = 1,
    B_VALUE_3950 = 2
}NTC_B_VALUE;

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
    uint16_t adc_processed_data[ADC_CHANNEL_NUM];
}ADC_DATA_BUFFER_ST;

void adc_task(void* pvParameters);

extern TaskHandle_t adc_task_handle;
