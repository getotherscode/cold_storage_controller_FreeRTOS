#include "main.h"
#include "stm32g0xx_hal_gpio.h"
#include "relay.h"

#include "FreeRTOS.h"
#include "task.h"

#define SET_COMPRESSOR_RELAY(PIN_STATE) HAL_GPIO_WritePin(COMPRESSOR_GPIO_Port, COMPRESSOR_Pin, (PIN_STATE))
#define TOGGLE_COMPRESSOR_STATE()       HAL_GPIO_TogglePin(COMPRESSOR_GPIO_Port, COMPRESSOR_Pin)
#define READ_COMPRESSOR_STATE()         HAL_GPIO_ReadPin(COMPRESSOR_GPIO_Port, COMPRESSOR_Pin)

#define SET_DEFROST_RELAY(PIN_STATE) HAL_GPIO_WritePin(DEFROST_GPIO_Port, DEFROST_Pin, (PIN_STATE))
#define TOGGLE_DEFROST_STATE()       HAL_GPIO_TogglePin(DEFROST_GPIO_Port, DEFROST_Pin)
#define READ_DEFROST_STATE()         HAL_GPIO_ReadPin(DEFROST_GPIO_Port, DEFROST_Pin)

#define SET_EVAPORATOR_FAN_RELAY(PIN_STATE) HAL_GPIO_WritePin(EVAPORATOR_FAN_GPIO_Port, EVAPORATOR_FAN_Pin, (PIN_STATE))
#define TOGGLE_EVAPORATOR_FAN_STATE()       HAL_GPIO_TogglePin(EVAPORATOR_FAN_GPIO_Port, EVAPORATOR_FAN_Pin)
#define READ_EVAPORATOR_FAN_STATE()         HAL_GPIO_ReadPin(EVAPORATOR_FAN_GPIO_Port, EVAPORATOR_FAN_Pin)

#define SET_HEATING_CABLE_RELAY(PIN_STATE) HAL_GPIO_WritePin(HEATING_CABLE_GPIO_Port, HEATING_CABLE_Pin, (PIN_STATE))
#define TOGGLE_HEATING_CABLE_STATE()       HAL_GPIO_TogglePin(HEATING_CABLE_GPIO_Port, HEATING_CABLE_Pin)
#define READ_HEATING_CABLE_STATE()         HAL_GPIO_ReadPin(HEATING_CABLE_GPIO_Port, HEATING_CABLE_Pin)

#define SET_ALARM_RELAY(PIN_STATE)  HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, (PIN_STATE))
#define TOGGLE_ALARM_RELAY()        HAL_GPIO_TogglePin(ALARM_GPIO_Port, ALARM_Pin)
#define READ_ALARM_STATE()          HAL_GPIO_ReadPin(ALARM_GPIO_Port, ALARM_Pin)

#define SET_LIQUID_SUPPLY_RELAY(PIN_STATE)  HAL_GPIO_WritePin(LIQUID_SUPPLY_GPIO_Port, LIQUID_SUPPLY_Pin, (PIN_STATE))
#define TOGGLE_LIQUID_SUPPLY_RELAY()            HAL_GPIO_TogglePin(LIQUID_SUPPLY_GPIO_Port, LIQUID_SUPPLY_Pin)
#define READ_LIQUID_SUPPLY_STATE()              HAL_GPIO_ReadPin(LIQUID_SUPPLY_GPIO_Port, LIQUID_SUPPLY_Pin)

#define SET_RELAY_7(PIN_STATE)       HAL_GPIO_WritePin(RELAY_7_GPIO_Port, RELAY_7_Pin, (PIN_STATE))
#define TOGGLE_RELAY_7_RELAY()       HAL_GPIO_TogglePin(RELAY_7_GPIO_Port, RELAY_7_Pin)
#define READ_RELAY_7_STATE()         HAL_GPIO_ReadPin(RELAY_7_GPIO_Port, RELAY_7_Pin)

#define SET_RELAY_8(PIN_STATE)       HAL_GPIO_WritePin(RELAY_8_GPIO_Port, RELAY_8_Pin, (PIN_STATE))
#define TOGGLE_RELAY_8_RELAY()       HAL_GPIO_TogglePin(RELAY_8_GPIO_Port, RELAY_8_Pin)
#define READ_RELAY_8_STATE()         HAL_GPIO_ReadPin(RELAY_8_GPIO_Port, RELAY_8_Pin)

void relay_ctrl(uint8_t relay_state, uint8_t relay_idx)
{
    switch (relay_idx)
    {
        case RELAY_COMPRESS:
            SET_COMPRESSOR_RELAY(relay_state);
            break;
            
        case RELAY_DEF:
            SET_DEFROST_RELAY(relay_state);
            break;
            
        case RELAY_EVAPORATOR:
            SET_EVAPORATOR_FAN_RELAY(relay_state);
            break;
            
        case RELAY_HEAT_CABLE:
            SET_HEATING_CABLE_RELAY(relay_state);
            break;
            
        case RELAY_ALARM:
            SET_ALARM_RELAY(relay_state);
            break;
            
        case RELAY_LQ_SPLY:
            SET_LIQUID_SUPPLY_RELAY(relay_state);
            break;
            
        case RELAY_7:
            SET_RELAY_7(relay_state);
            break;
            
        case RELAY_8:
            SET_RELAY_8(relay_state);
            break;
            
        default:
            break;
    }
}

uint8_t is_relay_on(uint8_t relay_idx)
{
    switch (relay_idx)
    {
        case RELAY_COMPRESS:
            return READ_COMPRESSOR_STATE();
            
        case RELAY_DEF:
            return READ_DEFROST_STATE();
            
        case RELAY_EVAPORATOR:
            return READ_EVAPORATOR_FAN_STATE();
            
        case RELAY_HEAT_CABLE:
            return READ_HEATING_CABLE_STATE();
            
        case RELAY_ALARM:
            return READ_ALARM_STATE();
            
        case RELAY_LQ_SPLY:
            return READ_LIQUID_SUPPLY_STATE();
            
        case RELAY_7:
            return READ_RELAY_7_STATE();
            
        case RELAY_8:
            return READ_RELAY_8_STATE();
            
        default:
            return 0;
    }
}

void relay_main(void *pvParameters)
{
    (void) pvParameters;

    for(;;)
    {
        for(uint8_t i = 0; i<8; i++)
        {
            relay_ctrl(RELAY_ON, i);
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        for(uint8_t i = 0; i<8; i++)
        {
            relay_ctrl(RELAY_OFF, i);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}
