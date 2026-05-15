#pragma once

#include "stdint.h"

#define COMPRESSOR_RELAY_OFF 0
#define COMPRESSOR_RELAY_ON  1

void compressor_ctrl(uint8_t cps_state);

void compressor_main(void *pvParameters);
