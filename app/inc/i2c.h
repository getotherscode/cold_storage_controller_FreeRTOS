#pragma once

#include <stdbool.h>
#include <stdint.h>

void i2c_start();

void i2c_stop();

void i2c_set_ack(const uint8_t ack_bit);

bool i2c_read_ack();

bool i2c_put_a_byte(uint8_t byte);

void i2c_get_a_byte(uint8_t* get_byte, bool give_ack);