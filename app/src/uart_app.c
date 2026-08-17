#include "uart_app.h"
#include "projdefs.h"
#include "stm32g0xx_hal_uart.h"
#include "usart.h"
#include "portmacro.h"
#include "crc.h"

#include <string.h>
#include <stdbool.h>

static UART_RX_ST uart2_rx_st;
static UART_RX_ST uart3_rx_st;
static UART_RX_ST uart4_rx_st;

static UART_TX_ST uart2_tx_st;
static UART_TX_ST uart3_tx_st;
static UART_TX_ST uart4_tx_st;

TaskHandle_t uart_recv_tasks_handler_t = NULL;

static inline void SET_UART4_TX(void) { HAL_GPIO_WritePin(UART4_485_LEFT_DIR_GPIO_Port, UART4_485_LEFT_DIR_Pin, GPIO_PIN_SET);}

static inline void SET_UART4_RX(void) { HAL_GPIO_WritePin(UART4_485_LEFT_DIR_GPIO_Port, UART4_485_LEFT_DIR_Pin, GPIO_PIN_RESET);}

void uart4_send(uint8_t* buffer, uint16_t len)
{
    memcpy(uart4_tx_st.send_buffer, buffer, len);

    SET_UART4_TX();
    HAL_UART_Transmit(&huart4, uart4_tx_st.send_buffer, len, 1000);
    SET_UART4_RX();
}

void set_uart2_recv_len(volatile uint16_t len)
{
    uart2_rx_st.recv_len = len;
}

void set_uart3_recv_len(volatile uint16_t len)
{
    uart3_rx_st.recv_len = len;
}

void set_uart4_recv_len(volatile uint16_t len)
{
    uart4_rx_st.recv_len = len;
}

static inline uint16_t get_uart2_recv_len(void)
{
    return uart2_rx_st.recv_len;
}

static inline uint16_t get_uart3_recv_len(void)
{
    return uart3_rx_st.recv_len;
}

static inline uint16_t get_uart4_recv_len(void)
{
    return uart4_rx_st.recv_len;
}

void reset_uart2_recv(void)
{
    uart2_rx_st.recv_len = 0;
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_rx_st.recv_buffer, UART_RECV_BYTES);
}

void reset_uart3_recv(void)
{
    uart3_rx_st.recv_len = 0;
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, uart3_rx_st.recv_buffer, UART_RECV_BYTES);
}

void reset_uart4_recv(void)
{
    uart4_rx_st.recv_len = 0;
    HAL_UARTEx_ReceiveToIdle_DMA(&huart4, uart4_rx_st.recv_buffer, UART_RECV_BYTES);
}

bool jump_to_bootloader(void)
{
    uint32_t boot_stack_top = *(uint32_t *)(BOOTLOADER_ADDR);
    if((boot_stack_top < SRAM_START_ADDR) || (boot_stack_top > (SRAM_START_ADDR + SRAM_SIZE)))
    {
        return false;
    }

    uint32_t reset_handler = *(uint32_t *)(BOOTLOADER_ADDR + 4);
    if((reset_handler < BOOTLOADER_ADDR) || (reset_handler > (BOOTLOADER_ADDR + BOOTLOADER_SIZE)))
    {
        return false;
    }

    SCB->VTOR = BOOTLOADER_ADDR;
    __set_MSP(boot_stack_top);
    void(*reset_app)(void) = (void(*)(void))reset_handler;
    reset_app();
    while(1);
}

void uart2_recv_tasks(void)
{
    const uint16_t len = get_uart2_recv_len();

    if(len > MESSAGE_MIN_LEN)
    {
        uint16_t get_crc = (uart2_rx_st.recv_buffer[len - 1] << 8) + uart2_rx_st.recv_buffer[len - 2];
        uint16_t cal_crc = get_crc16(CRC_INIT, uart2_rx_st.recv_buffer, len - 2);
        uint8_t id = uart2_rx_st.recv_buffer[0];
        uint8_t cmd = uart2_rx_st.recv_buffer[1];

        if(cal_crc == get_crc)
        {
            if(id == UART2_DEVICE_ID)
            {
                switch (cmd)
                {
                    case UART2_CMD_OTA:
                    {
                        jump_to_bootloader();
                    }
                }
            }
        }
    }
}

void uart3_recv_tasks(void)
{
    
}

void uart4_recv_tasks(void)
{
    
}

void uart_init(void)
{
    //turn on interrupt function
    reset_uart2_recv();
    reset_uart3_recv();
    reset_uart4_recv();
}

void uart_recv_tasks(void *pvParameters)
{
    (void) pvParameters;
    
    uint32_t event;

    for(;;)
    {
        // uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, TickType_t xTicksToWait
        xTaskNotifyWait(0, UINT32_MAX, &event, portMAX_DELAY);

        if(event & UART2_EVENT)
        {
            uart2_recv_tasks();
            reset_uart2_recv();
        }

        if(event & UART3_EVENT)
        {
            uart3_recv_tasks();
            reset_uart3_recv();
        }

        if(event & UART4_EVENT)
        {
            uart4_recv_tasks();
            reset_uart4_recv();
        }
    }
}


