#include "FreeRTOS.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h"
#include "stream_buffer.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "uart_app.h"

#define LOG_LEVEL LOG_LEVEL_DEBUG

#define LOG_BUF_SIZE      256
#define LOG_STREAM_SIZE   1024
#define LOG_TASK_STACK    256
#define LOG_TASK_PRIO     1

// IPC, transfer information streams between tasks and IT
static StreamBufferHandle_t log_stream = NULL;

static void log_task(void* args)
{
    (void) args;
    uint8_t chunk[64];
    for(;;)
    {
        size_t n = xStreamBufferReceive(log_stream, chunk, sizeof(chunk), portMAX_DELAY);
        if(n > 0)
        {
            uart4_send(chunk, (uint16_t)n);
        }
    }
}

void log_init(void)
{
    //one byte trigger
    log_stream = xStreamBufferCreate(LOG_STREAM_SIZE, 1);
    configASSERT(log_stream != NULL);
    xTaskCreate(log_task, "LOG", LOG_TASK_STACK,NULL, LOG_TASK_PRIO, NULL);
}

void log_print(const char* fmt, ...)
{
    char buff[LOG_BUF_SIZE];
    va_list args;

    va_start(args, fmt);
    //scan fmt, parse %d ... 
    int n = vsnprintf(buff,sizeof(buff),fmt,args);
    va_end(args);

    if(n < 0) {return;}
    if(n >= (int)sizeof(buff)) {n = sizeof(buff) - 1;}

    //if the context is iterrupt context
    if(xPortIsInsideInterrupt())
    {
        BaseType_t woken = pdFALSE;
        xStreamBufferSendFromISR(log_stream, buff, (size_t)n,&woken);
        portYIELD_FROM_ISR(woken);
    }
    else 
    {
        xStreamBufferSend(log_stream, buff, (size_t)n,0);
    }
}
