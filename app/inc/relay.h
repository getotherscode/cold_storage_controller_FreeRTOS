#pragma once

#include <stdint.h>

#define RELAY_OFF 0
#define RELAY_ON  1

typedef enum
{
    RELAY_COMPRESS   = 0,
    RELAY_DEF        = 1,
    RELAY_EVAPORATOR = 2,
    RELAY_HEAT_CABLE = 3,
    RELAY_ALARM      = 4,
    RELAY_LQ_SPLY    = 5,
    RELAY_7          = 6,
    RELAY_8          = 7
}RELAY_IDX_E;

void compressor_ctrl(uint8_t cps_state);

void relay_main(void *pvParameters);
