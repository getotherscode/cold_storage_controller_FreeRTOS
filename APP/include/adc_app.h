#pragma once

#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"

#define ADC_CHANNEL_NUM 12

void adc_task(void* pvParameters);

extern TaskHandle_t adc_task_handle;
