#include <stdint.h>
#include "tim.h"

void tim6_init()
{
    HAL_TIM_Base_Start(&htim6); 
}

void tim6_delay_us(uint32_t us)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(&htim6);
    while ((uint32_t)(__HAL_TIM_GET_COUNTER(&htim6) - start) < us);
}