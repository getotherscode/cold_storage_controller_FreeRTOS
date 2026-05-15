#pragma once

#include "stdint.h"

#define ALARM_RELAY_OFF 0
#define ALARM_RELAY_ON  1

void alarm_ctrl(uint8_t state);

void alarm_main(void *pvParameters);
