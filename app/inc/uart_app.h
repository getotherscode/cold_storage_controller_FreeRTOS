#pragma once

#include <stdint.h>
#include "main.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"
#include "stm32g0xx_hal_uart.h"
#include "FreeRTOS.h"
#include "task.h"


#define UART_RECV_BYTES 1024
#define UART_SEND_BYTES 1024

#define UART2_EVENT  (1 << 0)
#define UART3_EVENT  (1 << 1)
#define UART4_EVENT  (1 << 2)

#define UART2_DEVICE_ID 9

#define MESSAGE_MIN_LEN 6

typedef struct
{
    TaskHandle_t uart_task;
    uint8_t recv_buffer[UART_RECV_BYTES];
    volatile uint16_t recv_len;
}UART_RX_ST;

typedef struct
{
    uint8_t send_buffer[UART_SEND_BYTES];
    volatile uint16_t send_len;
}UART_TX_ST;

typedef enum 
{
    RIGHT_2   = 2,
    DISPLAY_3 = 3,
    LEFT_4    = 4,
} UART_ID;

typedef enum
{
    UART2_CMD_OTA = 1
}UART2_Cmd_Type;

TaskHandle_t* get_uart_task_handle_ptr(uint8_t uart_idth);

void uart4_send(uint8_t* buffer, uint16_t len);

void set_uart2_recv_len(volatile uint16_t len);

void set_uart3_recv_len(volatile uint16_t len);

void set_uart4_recv_len(volatile uint16_t len);

void uart_init(void);

void uart_recv_tasks(void *pvParameters);
