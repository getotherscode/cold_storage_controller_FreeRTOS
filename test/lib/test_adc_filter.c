#include "unity.h"
#include "adc_filter.h"

// insertion sort
void test_insertion_sort_normal(void)
{
    uint16_t arr_num = 6;
    uint16_t input[] = {0x0100,0x0000,0xFFFF,0x00F1,0xE100,0x0006};
    uint16_t expect[] = {0x0000,0x0006,0x00F1,0x0100,0xE100,0x0FFFF};

    insertion_sort(input, arr_num);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(expect, input, arr_num);
}

void test_insertion_sort_single_element(void)
{
    uint16_t arr_num = 1;
    uint16_t input[] = {0x0100};
    uint16_t expect[] = {0x0100};

    insertion_sort(input, arr_num);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(expect, input, arr_num);
}

void test_insertion_sort_repeat_element(void)
{
    uint16_t arr_num = 4;
    uint16_t input[] = {0x0100, 0x0100, 0x0100, 0x0100};
    uint16_t expect[] = {0x0100, 0x0100, 0x0100, 0x0100};

    insertion_sort(input, arr_num);
    TEST_ASSERT_EQUAL_UINT16_ARRAY(expect, input, arr_num);
}

// mean middle
void test_mean_middle_normal(void)
{
	uint16_t arr_num = 6;
	uint16_t trim_num = 2;
    uint16_t input[] = {0x0100,0x0000,0xFFFF,0x00F1,0xE100,0x0006};
    uint16_t expect = (0x00F1 + 0x0100)/(arr_num - trim_num*2);
	
	uint16_t result = mean_middle(input, arr_num, trim_num);
	TEST_ASSERT_EQUAL_UINT16(expect, result);
}

void test_mean_middle_no_trim(void)
{
	uint16_t arr_num = 2;
	uint16_t trim_num = 0;
    uint16_t input[] = {0x0100,0x0000};
    uint16_t expect = 0x0100/(arr_num - trim_num*2);
	
	uint16_t result = mean_middle(input, arr_num, trim_num);
	TEST_ASSERT_EQUAL_UINT16(expect, result);
}

void test_mean_middle_abnormal(void)
{
	uint16_t arr_num = 6;
	uint16_t trim_num = 3;
    uint16_t input[] = {0x0100,0x0000,0xFFFF,0x00F1,0xE100,0x0006};
    uint16_t expect = 0;
	
	uint16_t result = mean_middle(input, arr_num, trim_num);
	TEST_ASSERT_EQUAL_UINT16(expect, result);
}

// top k
void test_topk_normal(void)
{
	uint16_t arr_num = 6;
	uint16_t trim_num = 2;
    uint16_t input[] = {0x0100,0x0000,0xFFFF,0x00F1,0xE100,0x0006};
    uint16_t expect = (0xE100 + 0x0FFFF)/(arr_num - trim_num*2);
	
	uint16_t result = mean_topk(input, arr_num, trim_num);
	TEST_ASSERT_EQUAL_UINT16(expect, result);
}

void test_topk_no_trim(void)
{
	uint16_t arr_num = 2;
	uint16_t trim_num = 0;
    uint16_t input[] = {0x0100,0x0000};
    uint16_t expect = 0x0100/(arr_num - trim_num*2);
	
	uint16_t result = mean_topk(input, arr_num, trim_num);
	TEST_ASSERT_EQUAL_UINT16(expect, result);
}

void test_topk_abnormal(void)
{
	uint16_t arr_num = 6;
	uint16_t trim_num = 3;
    uint16_t input[] = {0x0100,0x0000,0xFFFF,0x00F1,0xE100,0x0006};
    uint16_t expect = 0;
	
	uint16_t result = mean_topk(input, arr_num, trim_num);
	TEST_ASSERT_EQUAL_UINT16(expect, result);
}

