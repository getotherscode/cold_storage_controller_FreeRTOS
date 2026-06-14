#include "unity.h"

//* for unity use
void setUp(void){}

void tearDown(void){}

//* adc_filter_test.c

//** insert sort
extern void test_insertion_sort_normal(void);

extern void test_insertion_sort_single_element(void);

extern void test_insertion_sort_repeat_element(void);

//** mean_middle
extern void test_mean_middle_normal(void);

extern void test_mean_middle_no_trim(void);

extern void test_mean_middle_abnormal(void);

//** mean_topk
extern void test_topk_normal(void);

extern void test_topk_no_trim(void);

extern void test_topk_abnormal(void);


int main()
{
	UNITY_BEGIN();
	
	RUN_TEST(test_insertion_sort_normal);
	RUN_TEST(test_insertion_sort_single_element);
	RUN_TEST(test_insertion_sort_repeat_element);
	
	RUN_TEST(test_mean_middle_normal);
	RUN_TEST(test_mean_middle_no_trim);
	RUN_TEST(test_mean_middle_abnormal);
	
	RUN_TEST(test_topk_normal);
	RUN_TEST(test_topk_no_trim);
	RUN_TEST(test_topk_abnormal);
	
	return UNITY_END();
}

