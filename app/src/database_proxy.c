#include "data_proxy.h"
#include "eep.h"

uint8_t EEP_DATA[512];

int16_t EEP_DATA_UPPER_LIMIT[512];

int16_t EEP_DATA_LOWER_LIMIT[512];

bool write_database(uint8_t start_id, uint8_t *data_chunck, uint16_t len)
{
    if(len == 0)
    {
        return false;
    }

    //data validate check
    //upper limit
    for(uint16_t i = 0; i<len-1; i++)
    {
        if(data_chunck[i] > EEP_DATA_UPPER_LIMIT[i])
        {
            return false;
        }
    }

    //lower limit
    for(uint16_t i = 0; i<len-1; i++)
    {
        if(data_chunck[i] < EEP_DATA_LOWER_LIMIT[i])
        {
            return false;
        }
    }

    if(is_eep_busy())
    {
        return false;
    }
    else
    {
        eep_write(start_id, data_chunck, len);
    }
}

uint8_t *read_database(uint8_t start_id, uint16_t len)
{
    if(len == 0)
    {
        return NULL;
    }

    uint8_t data_chunck[len];

    if(eep_read(start_id, data_chunck, len))
    {
        return data_chunck;
    }
    else
    {
        return NULL;
    }
}
