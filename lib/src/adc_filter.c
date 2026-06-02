#include "adc_filter.h"
#include "adc_app.h"


void insertion_sort(uint16_t *buffer, int n)
{
    //* insert sort
    //algrithmn understanding: image that a row of students, start from the first one, ask another student "are you older than me" who in front of this
    //if yes the asked students move back one step, if not, the asking student sit at the current position    
    for(int i = 1; i < n; i++)
    {
        uint16_t key = buffer[i];
        int16_t j = i - 1;
        while(j >= 0 && buffer[j] > key)
        {
            buffer[j + 1] = buffer[j];
            j--;
        }
        buffer[j + 1] = key;
    }
}

//temperature and pressure type data filter algrithom
uint16_t mean_middle(uint16_t* buffer, uint16_t trim_num)
{

    insertion_sort(buffer, SAMPLE_BUFFER_NUM);

    //trim at both sides and get the average
    uint32_t sum = 0;
    uint16_t average;
    for(int i = trim_num; i < SAMPLE_BUFFER_NUM - trim_num; i++)
    {
        sum += buffer[i];
    }

    average = sum / (SAMPLE_BUFFER_NUM - trim_num * 2);
    return average;
}

//current type data algrithom
uint16_t mean_topk(uint16_t* buffer, uint16_t trim_num)
{
    insertion_sort(buffer, SAMPLE_BUFFER_NUM);

    //trim at both sides and get the average
    uint32_t sum = 0;
    uint16_t average;
    for(int i = trim_num * 2; i < SAMPLE_BUFFER_NUM; i++)
    {
        sum += buffer[i];
    }

    average = sum / (SAMPLE_BUFFER_NUM - trim_num * 2);
    return average;
}

