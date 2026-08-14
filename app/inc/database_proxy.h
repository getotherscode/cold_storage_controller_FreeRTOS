#pragma once

typedef enum
{
    D1 = 0,
    D2 = 1,
    D3 = 2,
    D4 = 3,
    D5 = 4
}DATA_IDX;

bool write_database(uint8_t start_id, uint8_t *data_chunck, uint16_t len);

uint8_t *read_database(uint8_t start_id, uint16_t len);
