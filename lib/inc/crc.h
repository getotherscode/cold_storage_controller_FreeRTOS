#pragma once
#include <stdint.h>

#define CRC_INIT 0xFFFF

uint16_t get_crc16(uint16_t crc, const uint8_t *data, uint32_t len);
