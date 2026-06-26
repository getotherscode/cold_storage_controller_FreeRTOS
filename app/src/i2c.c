#include "i2c.h"
#include "log.h"
#include "stm32g0xx_hal_gpio.h"
#include "main.h"
#include "timer.h"

#define I2C_FAST_MODE_400KHZ 1

static inline void scl_high() {HAL_GPIO_WritePin(EEP_SCL_GPIO_Port, EEP_SCL_Pin, GPIO_PIN_SET);}

static inline void scl_low() {HAL_GPIO_WritePin(EEP_SCL_GPIO_Port, EEP_SCL_Pin, GPIO_PIN_RESET);}

static inline void sda_high() {HAL_GPIO_WritePin(EEP_SDA_GPIO_Port, EEP_SDA_Pin, GPIO_PIN_SET);}

static inline void sda_low() {HAL_GPIO_WritePin(EEP_SDA_GPIO_Port, EEP_SDA_Pin, GPIO_PIN_RESET);}

static inline GPIO_PinState read_sda() { return HAL_GPIO_ReadPin(EEP_SDA_GPIO_Port, EEP_SDA_Pin);}

//DWT or TIM
static inline void i2c_delay() 
{
#ifdef I2C_FAST_MODE_400KHZ
    tim6_delay_us(1);
#else
    tim6_delay_us(5);
#endif
}

static inline void scl_high_with_delay() {scl_high(); i2c_delay();}

static inline void scl_low_with_delay() {scl_low(); i2c_delay();}

static inline void sda_high_with_delay() {sda_high(); i2c_delay();}

static inline void sda_low_with_delay() {sda_low(); i2c_delay();}

static inline void set_ack() { scl_high_with_delay(); sda_low_with_delay(); }

void i2c_start()
{
    scl_low_with_delay();
    sda_high_with_delay();
    scl_high_with_delay();
    sda_low_with_delay();
}

void i2c_stop()
{
    scl_low_with_delay();
    sda_low_with_delay();
    scl_high_with_delay();
    sda_high_with_delay();
}

static void i2c_put_bit(const uint8_t bit)
{
    scl_low_with_delay();

    if(bit)
    {
        sda_high_with_delay();
    }
    else
    {
        sda_low_with_delay();
    }

    scl_high_with_delay();
}

bool i2c_read_ack()
{
    //for reading ack, release sda
    scl_low_with_delay();
    sda_high_with_delay();
    scl_high_with_delay();

    bool ack = (read_sda() == GPIO_PIN_RESET);

    scl_low_with_delay();
    
    return ack;
}

bool i2c_put_a_byte(uint8_t byte)
{
    for(uint8_t i = 0; i<8; i++)
    {
        //get highest bit
        if(byte & 0x80)
        {
            i2c_put_bit(1);
        }
        else
        {
            i2c_put_bit(0);
        }

        byte <<= 1;
    }

    bool read_ret = i2c_read_ack();

    return read_ret;
}

void i2c_get_a_byte(uint8_t* get_byte, bool give_ack)
{
    uint8_t ret = 0;

    for(uint8_t i = 0; i<8; i++)
    {
        scl_high_with_delay();

        //get lowest bit
        if(read_sda())
        {
            ret += (1 << (7-i));
        }

        scl_low_with_delay();
    }

    if(give_ack)
    {
        set_ack();
    }
    else
    {
        i2c_stop();
    }

    *get_byte = ret;
}

