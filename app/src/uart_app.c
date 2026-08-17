#include "uart_app.h"
#include "projdefs.h"
#include "stm32g0xx_hal_uart.h"
#include "usart.h"
#include "portmacro.h"

#include <string.h>

static UART_RX_ST uart2_rx_st;
static UART_RX_ST uart3_rx_st;
static UART_RX_ST uart4_rx_st;

static UART_TX_ST uart2_tx_st;
static UART_TX_ST uart3_tx_st;
static UART_TX_ST uart4_tx_st;

TaskHandle_t* get_uart_task_handle_ptr(uint8_t uart_idth)
{
    switch (uart_idth)
    {
        case 2:{return &uart2_rx_st.uart_task;}break;
        case 3:{return &uart3_rx_st.uart_task;}break;
        case 4:{return &uart4_rx_st.uart_task;}break;
    }
    return NULL;
}

void uart_idel_it_callback(UART_HandleTypeDef *huart)
{
    BaseType_t woken = pdFALSE;

    if(huart->Instance == USART2)
    {
        uart2_rx_st.recv_len = huart->RxXferSize - huart->RxXferCount;
        vTaskNotifyGiveFromISR(uart2_rx_st.uart_task, &woken);
    }

    if(huart->Instance == USART3)
    {
        uart3_rx_st.recv_len = huart->RxXferSize - huart->RxXferCount;
        vTaskNotifyGiveFromISR(uart3_rx_st.uart_task, &woken);
    }

    if(huart->Instance == USART4)
    {
        uart4_rx_st.recv_len = huart->RxXferSize - huart->RxXferCount;
        vTaskNotifyGiveFromISR(uart4_rx_st.uart_task, &woken);
    }

    portYIELD_FROM_ISR(woken);
}

void uart4_send(uint8_t* buffer, uint16_t len)
{
    memcpy(uart4_tx_st.send_buffer, buffer, len);

    SET_UART4_TX();
    HAL_UART_Transmit(&huart4, uart4_tx_st.send_buffer, len, 1000);
    SET_UART4_RX();
}

void uart2_recv_tasks(void *pvParameters)
{
    (void)pvParameters;
    
    while(1)
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

void uart3_recv_tasks(void *pvParameters)
{
    (void)pvParameters;
    
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
    }

}

void uart4_recv_tasks(void *pvParameters)
{
    (void)pvParameters;
    
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
    }

}

void uart_init(void)
{
    //turn on interrupt function
    HAL_UART_Receive_IT(&huart2, uart2_rx_st.recv_buffer, UART_RECV_BYTES);
    HAL_UART_Receive_IT(&huart3, uart3_rx_st.recv_buffer, UART_RECV_BYTES);
    HAL_UART_Receive_IT(&huart4, uart4_rx_st.recv_buffer, UART_RECV_BYTES);

    //specify iterrupt type: IDEL
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
}


