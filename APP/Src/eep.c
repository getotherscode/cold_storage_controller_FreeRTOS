/*   EEP FEATURE   */
//CHIP-SIZE: 512 bytes
//PAGE-SIZE: 16

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "i2c.h"
#include "log.h"

//ID 0b1010, chip enable address 0b00x(x is for addressing), bit0 is R/W
static const uint8_t EEP_ID = 0x0A;

//address is from 0 - 511
//return: write success or write fail
bool eep_write_one_byte(const uint16_t address, const uint8_t data)
{
    if(address > 511)
    {
        return false;
    }

    //device select code + W
    uint8_t dev_slct_code = EEP_ID << 4;
    if(address >= UINT8_MAX)
    {
        //bit0 - bit3 : 0010
        dev_slct_code += 2; 
    }
    // else bit0 - bit3 : 0000

    i2c_start();

    if(!i2c_put_a_byte(dev_slct_code))
    {
        i2c_stop();
        return false;
    }

    //address
    if(!i2c_put_a_byte(address))
    {
        i2c_stop();
        return false;
    }

    //data
    if(!i2c_put_a_byte(data))
    {
        i2c_stop();
        return false;
    }

    i2c_stop();

    return true;
}

bool eep_random_read_one_byte(const uint16_t address, uint8_t* ret)
{
    if(address > 511)
    {
        return false;
    }

    //device select code + W to get to target address
    uint8_t dev_slct_code = EEP_ID << 4;
    if(address >= UINT8_MAX)
    {
        //bit0 - bit3 : 0010
        dev_slct_code += 2; 
    }
    // else bit0 - bit3 : 0000

    i2c_start();
    
    if(!i2c_put_a_byte(dev_slct_code))
    {
        i2c_stop();
        return false;
    }

    //address
    if(!i2c_put_a_byte(address))
    {
        i2c_stop();
        return false;        
    }

    //device select code + R
    dev_slct_code = EEP_ID << 4;
    if(address >= UINT8_MAX)
    {
        //bit0 - bit3 : 0011
        dev_slct_code += 3; 
    }
    else
    {
        //bit0 - bit3 : 0001
        dev_slct_code += 1; 
    }
    
    i2c_start();
    
    if(!i2c_put_a_byte(dev_slct_code))
    {
        i2c_stop();
        return false;
    }

    i2c_get_a_byte(ret, false);

    i2c_stop();
    
    return true;
}



/*byte read and byte write test*/
// 1- use oscilloscope verify wave
// 2- test 100khz and 40khz
/* void eep_one_byte_test()
{
    const uint8_t address = 2;
    uint8_t write_byte = 0xF;
    uint8_t read_byte = 0;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(1000);  // 1ms

    for(;;)
    {
        xTaskDelayUntil(&xLastWakeTime, xPeriod);

        if(!eep_write_one_byte(address, write_byte))
        {
            LOG_ERROR("write address %d is failed", address);
        }

        vTaskDelay(10);

        if(!eep_random_read_one_byte(address, &read_byte))
        {
            LOG_ERROR("read address %d is failed", address);
        }
        else
        {
            LOG_INFO("read address %d is %d", address, read_byte);
        }
    }
} */


