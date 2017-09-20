/*
	File name : test_circbuff.c
	File Description : Source files unit tests for Circular Buffer
	File Author: Vihanga Bare
	Assignment Date : 9/18/2017
	Tools used : Ubuntu 16.04.2, GCC version 5.4.0, cmocka, cmake	
*/

#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdint.h>
#include <stdio.h>

#include "circbuff.h"

/*Function - Unit test to check successful allocation of memory for circular buffer */
void test_cbuff_malloc_success(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	
	cbuff_stat status = cbuffMalloc(&head,5);
        assert_int_equal(status,AVAILABLE);
}

/*Function - Unit test to if failed allocation of memory for circular buffer returns error*/
void test_cbuff_malloc_fail(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	
	cbuff_stat status = cbuffMalloc(&head,-1);
        assert_int_equal(status,ERROR);
}
/*Function - Unit test to check if attempt to add to an empty buffer returns error */
void test_cbuff_add_invalid_pos(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	
	cbuff_stat status = cbuffAdd(&head,5,1);
        assert_int_equal(status,ERROR);
}

/*Function - Unit test to check if count to add given as 0 to circular buffer returns error*/
void test_cbuff_add_invalid_count(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	
	cbuff_stat status = cbuffAdd(&head,5,0);
        assert_int_equal(status,ERROR);
}

/*Function - Unit test to check success in adding elements to available circular buffer*/
void test_cbuff_add_when_bufferavailable(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	cbuffMalloc(&head,4);
	cbuffInit(&head,4);	
	cbuffAdd(&head,5,1);
	cbuffAdd(&head,6,1);
	cbuff_stat status = cbuffAdd(&head,7,1);
        assert_int_equal(status,AVAILABLE);
}

/*Function - Unit test to check adding to a full circular buffer returns error*/
void test_cbuff_add_when_bufferfull(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	cbuffMalloc(&head,4);
	cbuffInit(&head,4);	
	cbuffAdd(&head,5,1);
	cbuffAdd(&head,6,1);
	cbuffAdd(&head,7,1);
	cbuffAdd(&head,8,1);
	cbuff_stat status = cbuffAdd(&head,9,1);
        assert_int_equal(status,BUFFER_FULL);
}
/*Function - Unit test to if buffer full status is returned sucessfully */
void test_cbuff_checkif_bufferfull(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	cbuffMalloc(&head,4);
	cbuffInit(&head,4);	
	cbuffAdd(&head,5,1);
	cbuffAdd(&head,6,1);
	cbuffAdd(&head,7,1);
	cbuffAdd(&head,8,1);
	cbuffAdd(&head,9,1);
	cbuff_stat status = cbuffFull(&head);
        assert_int_equal(status,BUFFER_FULL);
}

/*Function - Unit test to check success of removing elements from available circular buffer */
void test_cbuff_remove_when_bufferavailable(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	uint32_t value;

	cbuffMalloc(&head,4);
	cbuffInit(&head,4);	
	cbuffAdd(&head,5,1);
	cbuffAdd(&head,6,1);
	cbuffAdd(&head,7,1);
	
	cbuffRemove(&head,&value,1);
	//printf("data is ......%d", value);
        assert_int_equal(value,5);
}
/*Function - Unit test to check if remove element count as 0 returns error */
void test_cbuff_remove_invalid_count(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	uint32_t data;
	cbuffMalloc(&head,4);
	cbuffInit(&head,4);	
	cbuffAdd(&head,5,1);
	cbuffAdd(&head,6,1);
	cbuffAdd(&head,7,1);
	cbuffAdd(&head,8,1);
	cbuff_stat status = cbuffRemove(&head,&data,0);
        assert_int_equal(status,ERROR);
}

/*Function - Unit test to check if removing elements from empty circular buffer returns error*/
void test_cbuff_remove_when_bufferempty(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	uint32_t data;
	cbuffMalloc(&head,4);
	cbuffInit(&head,4);	
	cbuffAdd(&head,5,1);
	cbuffAdd(&head,6,1);
	cbuffAdd(&head,7,1);
	cbuffAdd(&head,8,1);
	
	cbuffRemove(&head,&data,1);
	cbuffRemove(&head,&data,1);
	cbuffRemove(&head,&data,1);	
	cbuffRemove(&head,&data,1);
	cbuff_stat status = cbuffRemove(&head,&data,1);
        assert_int_equal(status,BUFFER_EMPTY);
}
/*Function - Unit test to check if remove from circular buffer not yet created returns error*/
void test_cbuff_remove_invalid_pos(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	uint32_t data;
	
	cbuff_stat status = cbuffRemove(&head,&data,1);
        assert_int_equal(status,ERROR);
}

/*Function - Unit test to check if remove from circular buffer not yet created returns error*/
void test_cbuff_checkif_bufferempty(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	uint32_t data;

	cbuffMalloc(&head,4);
	cbuffInit(&head,4);	
	cbuffAdd(&head,5,1);
	cbuffAdd(&head,6,1);
	cbuffAdd(&head,7,1);
	cbuffAdd(&head,8,1);
	
	cbuffRemove(&head,&data,1);
	cbuffRemove(&head,&data,1);
	cbuffRemove(&head,&data,1);	
	cbuffRemove(&head,&data,1);
	cbuff_stat status = cbuffEmpty(&head);
        assert_int_equal(status,BUFFER_EMPTY);
}
/*Function - Unit test to check success in deleting buffer*/
void test_cbuff_delete(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	cbuffMalloc(&head,4);
	cbuffInit(&head,4);	
	cbuffAdd(&head,5,1);
	cbuffAdd(&head,6,1);
	cbuffAdd(&head,7,1);
	cbuffAdd(&head,8,1);
	
	cbuff_stat status = cbuffDelete(&head);
        assert_int_equal(status,SUCCESS);
}
/*Function - Unit test to check if printing empty buffer returns error*/
void test_cbuff_print_empty(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;

	cbuff_stat status = cbuffPrint(&head);
        assert_int_equal(status,ERROR);
}

/*Function - Unit test to check size of empty buffer returns ZERO*/
void test_cbuff_sizeof_empty(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;

	uint32_t size = sizeBuff(&head);
        assert_int_equal(0,size);
}

/*Function - Unit test to check success in printing an available empty buffer*/
void test_cbuff_print_valid(void **state)
{
  	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	cbuffMalloc(&head,4);
	cbuffInit(&head,4);	
	cbuffAdd(&head,5,1);
	cbuffAdd(&head,6,1);
	cbuffAdd(&head,7,1);
	cbuffAdd(&head,8,1);
	cbuff_stat status = cbuffPrint(&head);
        assert_int_equal(status,SUCCESS);
}

int main(int argc, char **argv)
{
  const struct CMUnitTest tests[] = 
  {
	//unit test framework struct	
	cmocka_unit_test(test_cbuff_malloc_success),
	cmocka_unit_test(test_cbuff_malloc_fail),
	cmocka_unit_test(test_cbuff_add_invalid_pos),
	cmocka_unit_test(test_cbuff_add_invalid_count),
	cmocka_unit_test(test_cbuff_add_when_bufferavailable),
	cmocka_unit_test(test_cbuff_add_when_bufferfull),
	cmocka_unit_test(test_cbuff_checkif_bufferfull),
	cmocka_unit_test(test_cbuff_remove_when_bufferavailable),
	cmocka_unit_test(test_cbuff_remove_when_bufferempty),
	cmocka_unit_test(test_cbuff_remove_invalid_pos),
	cmocka_unit_test(test_cbuff_checkif_bufferempty),
	cmocka_unit_test(test_cbuff_delete),
	cmocka_unit_test(test_cbuff_print_empty), 
	cmocka_unit_test(test_cbuff_sizeof_empty),
        cmocka_unit_test(test_cbuff_print_valid), 
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
