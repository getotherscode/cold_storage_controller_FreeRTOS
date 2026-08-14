#pragma once

#include "eep.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t cur_buf_index;
    uint16_t left_bytes;
    uint8_t state;

    bool is_busy;           //write task finished flag, can not be set
} EEP_WRITE_ST;

typedef enum
{
    EEP_WRITE_INIT = 0,
    EEP_WRITE,
    EEP_FINISH
} EEP_STATE;

bool eep_write(const uint16_t start_address, uint8_t* buffer, const uint16_t len);

bool eep_read(const uint16_t start_address, uint8_t* buffer, const uint16_t len);

bool is_eep_busy(void);

void eep_test(void* pvParameters);
