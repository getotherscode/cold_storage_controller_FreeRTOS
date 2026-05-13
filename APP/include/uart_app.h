#pragma once

#include <stdint.h>
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_uart.h"
#include "FreeRTOS.h"
#include "task.h"


#define UART_RECV_BYTES 1024

typedef struct
{
    TaskHandle_t uart_task;
    uint8_t recv_buffer[UART_RECV_BYTES];
    volatile uint16_t recv_len;
}UART_RX_ST;

typedef enum 
{
    RIGHT = 2,
    DISPLAY = 3,
    LEFT = 4,
} UART_ID;

TaskHandle_t* get_uart_task_handle_ptr(uint8_t uart_idth);

void uart_idel_it_callback(UART_HandleTypeDef *huart);

void uart2_recv_tasks(void *pvParameters);

void uart3_recv_tasks(void *pvParameters);

void uart4_recv_tasks(void *pvParameters);

void uart_init(void);
