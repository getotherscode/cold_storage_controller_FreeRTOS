/*   EEP FEATURE   */
//CHIP-SIZE: 512 bytes
//PAGE-SIZE: 16
//PAGE-NUM : 32
//PAGE-HEAD-ADDR: 0, 0x10, 0x20, 0x30 ... 0x100, 0x110，0x120, ... 0x1F0

#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "i2c.h"
#include "log.h"
#include "eep.h"

static const uint8_t EEP_ID   = 0x0A;  //ID 0b1010, chip enable address 0b00x(x is for addressing), bit0 is R/W
static const uint16_t EEP_END = 511;   //address is from 0 - 511

static EEP_WRITE_ST eep_w_st;

//TODO: use oscillscope to verify encapsulate is not influence the i2c driver wave
static bool eep_write_init(const uint16_t start_address)
{
    if(start_address > EEP_END)
    {
        return false;
    }

    //device select code + W
    uint8_t dev_slct_code = EEP_ID << 4;
    if(start_address >= UINT8_MAX)
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
    if(!i2c_put_a_byte(start_address))
    {
        i2c_stop();
        return false;
    }

    return true;
}

static bool eep_read_init(const uint16_t start_address)
{
    //device select code + R
    uint8_t dev_slct_code = EEP_ID << 4;
    if(start_address >= UINT8_MAX)
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

    return true;
}

//return: write success or write fail
bool eep_write_one_byte(const uint16_t start_address, const uint8_t data)
{
    if(!eep_write_init(start_address))
    {
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

/*  - page write, max 16 bytes written in same page (could not beyond the page)
    - use address A4-bit to determine if beyonded the page in one written
*/
bool eep_page_random_write(const uint16_t start_address, uint8_t* buffer, const uint8_t len)
{
    uint16_t start_bit4 = (start_address & 0x0010);
    uint16_t end_address = (start_address + len - 1);
    uint16_t end_bit4 = (end_address & 0x0010);
    bool in_same_page = (start_bit4 == end_bit4);

    if(len > 16)
    {
        return false;
    }

    if(end_address > EEP_END)
    {
        return false;
    }

    if(!in_same_page)
    {
        return false;
    }

    if(!eep_write_init(start_address))
    {
        return false;
    }

    for(uint8_t index = 0; index < len ; index++)
    {
        if(!i2c_put_a_byte(buffer[index]))
        {
            return false;
        }
    }

    i2c_stop();

    return true;
}

bool eep_write(const uint16_t start_address, uint8_t* buffer, const uint16_t len)
{
    switch(eep_w_st.state)
    {
        case EEP_WRITE_INIT:
        {
            uint16_t end_address = (start_address + len - 1);

            if(end_address > EEP_END)
            {
                return false;
            }

            eep_w_st.cur_buf_index = 0;
            eep_w_st.left_bytes = len;
            eep_w_st.state = EEP_WRITE;
            eep_w_st.is_busy = true;
        }
        break;

        case EEP_WRITE:
        {
            if(eep_w_st.left_bytes == 0)
            {
                eep_w_st.state = EEP_FINISH;
                eep_w_st.is_busy = false;
                break;
            }

            //cross page have limit
            uint16_t cur_start_addr = start_address + eep_w_st.cur_buf_index;
            uint16_t next_page_head = (cur_start_addr & 0x0010) + 0x10;
            uint8_t cur_page_availble = next_page_head - cur_start_addr;
            
            if(cur_page_availble > eep_w_st.left_bytes)
            {
                cur_page_availble = eep_w_st.left_bytes;
            }

            uint8_t temp_buffer[16];
            memcpy(temp_buffer, buffer + eep_w_st.cur_buf_index, cur_page_availble);

            if(!eep_page_random_write(cur_start_addr, temp_buffer, cur_page_availble))
            {
                return false;
            }
            else
            {
                //update
                eep_w_st.cur_buf_index += cur_page_availble;
                eep_w_st.left_bytes -= cur_page_availble;
            }
        }
        break;

        case EEP_FINISH:
        {
            eep_w_st.cur_buf_index = 0;
        }
        break;
    }

    return true;
}

bool is_eep_busy(void)
{
    return eep_w_st.is_busy;
}

bool eep_read(const uint16_t start_address, uint8_t* buffer, const uint16_t len)
{
    uint16_t end_address = (start_address + len - 1);

    if(end_address > EEP_END)
    {
        return false;
    }
    
    if(!eep_write_init(start_address))
    {
        return false;
    }

    if(!eep_read_init(start_address))
    {
        return false;
    }

    for(uint16_t counter = 0; counter < len; counter++)
    {
        bool give_ack = (counter == (len - 1)) ? false : true;
        i2c_get_a_byte(buffer + counter, give_ack);
    }

    i2c_stop();
    
    return true;
}

/*byte read and byte write test*/
// 1- use oscilloscope verify wave
// 2- test 100khz and 40khz
void eep_test(void* pvParameters)
{
    (void)pvParameters;
    uint8_t test_write[16] = {0x01, 0x02, 0x03, 0x04, 0, 0, 0, 0x0A, 0, 0, 0, 0, 0x05, 0x06, 0x07, 0x09};
    uint8_t test_read[16] = {0};
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(5000);  // 5s

    for(;;)
    {
        xTaskDelayUntil(&xLastWakeTime, xPeriod);

        /* not cross page */
        //uint16_t start_address = 0;
        
        /* cross page */
        uint16_t start_address = 5;

        if(!eep_write(start_address, test_write, sizeof(test_write)))
        {
            LOG_ERROR("write is failed\n");
        } 

        vTaskDelay(100);

        if(!eep_read(start_address, test_read, 16))
        {
            LOG_ERROR("read is failed\n");
        }
        else
        {
            for(uint8_t i = 0; i < 16; i++)
            {
                LOG_DEBUG("%d", test_read[i]);
            }
        }
    }
}


