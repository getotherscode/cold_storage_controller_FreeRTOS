#include "alarm.h"
#include "main.h"

#define TRUN_ON_ALARM_RELAY()  HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_SET)
#define TRUN_OFF_ALARM_RELAY() HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET)
#define TOGGLE_ALARM_RELAY() HAL_GPIO_TogglePin(ALARM_GPIO_Port, ALARM_Pin)

void alarm_ctrl(uint8_t state)
{
    if(state == ALARM_RELAY_ON)
    {
        TRUN_ON_ALARM_RELAY();
    }
    else if(state == ALARM_RELAY_OFF)
    {
        TRUN_OFF_ALARM_RELAY();
    }
}

void alarm_main(void *pvParameters)
{
    (void) pvParameters;
}
