#include "main.h"
#include "stm32g0xx_hal_gpio.h"
#include "compressor.h"
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#define TRUN_ON_COMPRESSOR()  HAL_GPIO_WritePin(COMPRESSOR_GPIO_Port, COMPRESSOR_Pin, GPIO_PIN_SET)
#define TRUN_OFF_COMPRESSOR() HAL_GPIO_WritePin(COMPRESSOR_GPIO_Port, COMPRESSOR_Pin, GPIO_PIN_RESET)
#define TOGGLE_COMPRESSORS_STATE() HAL_GPIO_TogglePin(COMPRESSOR_GPIO_Port, COMPRESSOR_Pin)

void compressor_ctrl(uint8_t cps_state)
{
    if(cps_state ==COMPRESSOR_RELAY_ON)
    {
        TRUN_ON_COMPRESSOR();
    }
    else if(cps_state ==COMPRESSOR_RELAY_OFF)
    {
        TRUN_OFF_COMPRESSOR();
    }
}

void compressor_main(void *pvParameters)
{
    (void) pvParameters;
    for(;;)
    {
        TOGGLE_COMPRESSORS_STATE();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
