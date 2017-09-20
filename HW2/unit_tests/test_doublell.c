/*
	File name : test_doublell.c
	File Description : Source files unit tests for Double Linked List
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

#include "doublell.h"

/*Function - Unit test to check successful insert at beginning of DLL*/
void test_insert_beginning(void **state)
{
  
   //create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	uint32_t data,position;
	data = 15;
	position = 1;
	dll_stat status = insertNth(&head, position, data);
	assert_int_equal(status,SUCCESS);
}

/*Function - Unit test to check if an attempt made to add to an empty DLL returns INVALID INDEX error*/
void test_add_invalid_pos(void **state)
{
  
  //create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	uint32_t data,position;
	data = 5;
	position = 5;
	dll_stat status = insertNth(&head, position, data);
	assert_int_equal(status,INVALID_INDEX);
}

/*Function - Unit test to check if success in adding to end of DLL*/
void test_insert_at_end(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	uint32_t data,position;
	data = 15;
	position = 1;
	
	insertNth(&head,position,data);
	data = 16;
	position = 2;

	dll_stat status = insertNth(&head, position, data);
	assert_int_equal(status,SUCCESS);
}
/*Function - Unit test to check if successful add in the middle of DLL */
void test_insert_in_between(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	uint32_t data,position;
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;
	insertNth(&head,position,data);
        	
	data = 17;
	dll_stat status = insertNth(&head, position, data);
	assert_int_equal(status,SUCCESS);
}

/*Function - Unit test to check if removing from an empty DLL returns Invalid index error*/
void test_remove_from_emptylist(void **state)
{
  
   //create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	uint32_t position,data;

	position = 2;
	dll_stat status = deleteNth(&head, position, &data);
	assert_int_equal(status,INVALID_INDEX);
}

/*Function - Unit test to check successful remove from beginning of DLL */
void test_remove_from_beginning(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	uint32_t data,position;
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;
	insertNth(&head,position,data);
        	
	position = 1;
	dll_stat status = deleteNth(&head, position, &data);
	assert_int_equal(status,SUCCESS);
}

/*Function - Unit test to check successful remove from the end of DLL */
void test_remove_from_end(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	uint32_t data,position;
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;
	insertNth(&head,position,data);
        	
	data = 17;
	position = 3;
	insertNth(&head,position,data);

	position = 3;
	dll_stat status = deleteNth(&head, position, &data);
	assert_int_equal(status,SUCCESS);
}
/*Function - Unit test to check successful remove from the middle of DLL */
void test_remove_within(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	uint32_t data,position;
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;
	insertNth(&head,position,data);
        	
	data = 17;
	position = 3;
	insertNth(&head,position,data);

	position = 2;
	dll_stat status = deleteNth(&head, position, &data);
	assert_int_equal(status,SUCCESS);
}

/*Function - Unit test to check if attempt to remove from position value greater than size of DLL returns invalid index error */
void test_remove_invalid_pos(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	uint32_t data,position;
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;
	insertNth(&head,position,data);
        	
	data = 17;
	position = 3;
	insertNth(&head,position,data);

	position = 5;
	dll_stat status = deleteNth(&head, position, &data);
	assert_int_equal(status,INVALID_INDEX);
}
/*Function - Unit test to check can successfully search element present in DLL */
void test_search_not_empty(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;

	uint32_t data,position;
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;

	insertNth(&head,position,data);
        	
	data = 17;
	position = 3;
	insertNth(&head,position,data);

	data=17;

	searchDLL(&head, data, &position);
	assert_int_equal(position,3);
}

/*Function - Unit test to check if data not found in DLL returns error */
void test_search_not_found(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;

	uint32_t data,position;
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;

	insertNth(&head,position,data);
        	
	data = 17;
	position = 3;
	insertNth(&head,position,data);

	data=22;

	dll_stat status = searchDLL(&head, data, &position);
	assert_int_equal(status,ERROR);
}
/*Function - Unit test to check search in empty DLL returns error*/
void test_search_in_empty(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;

	uint32_t data,position;

	data=1;
	position=1;

	dll_stat status = searchDLL(&head, data, &position);
	assert_int_equal(status,ERROR);
}
/*Function - Unit test to check if adding at position greater than size of DLL returns invali dindex error */
void test_add_invalid_end_pos(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;

	uint32_t data,position;
	
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;

	insertNth(&head,position,data);
        	
	data = 17;
	position = 3;
	insertNth(&head,position,data);

	data = 18;	
	position = 10;

	dll_stat status = insertNth(&head,position,data);

	assert_int_equal(status,INVALID_INDEX);
}

/*Function - Unit test to check if correct size of DLL is calculated*/
void test_size_valid(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;

	uint32_t data,position;
	
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;

	insertNth(&head,position,data);
        	
	data = 17;
	position = 3;
	insertNth(&head,position,data);

	uint32_t size = sizeDLL(&head);

	assert_int_equal(position,size);
}

/*Function - Unit test to check if printing an empty list returns error */
void test_print_invalid(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;
	dll_stat status = printDLL(head);

	assert_int_equal(status,ERROR);
}

/*Function - Unit test to succesfully printing DLL which is not empty */
void test_print_valid(void **state)
{
  	//create a new element for DLL and allocate space for it using malloc
  	struct NodeDLL* head = NULL;

	uint32_t data,position;
	
	data = 15;
	position = 1;
	insertNth(&head,position,data);

	data = 16;
	position = 2;

	insertNth(&head,position,data);
        	
	data = 17;
	position = 3;
	insertNth(&head,position,data);

	dll_stat status = printDLL(head);

	assert_int_equal(status,SUCCESS);
}

int main(int argc, char **argv)
{
  const struct CMUnitTest tests[] = 
  {
	//unit test framework struct
	cmocka_unit_test(test_add_invalid_pos),
	cmocka_unit_test(test_insert_beginning),
	cmocka_unit_test(test_insert_at_end),
	cmocka_unit_test(test_insert_in_between),
	cmocka_unit_test(test_remove_from_emptylist),
	cmocka_unit_test(test_remove_from_beginning),
	cmocka_unit_test(test_remove_from_end),
	cmocka_unit_test(test_remove_within),
	cmocka_unit_test(test_remove_invalid_pos),
	cmocka_unit_test(test_search_in_empty),
	cmocka_unit_test(test_search_not_empty),	
	cmocka_unit_test(test_search_not_found),
	cmocka_unit_test(test_add_invalid_end_pos),
	cmocka_unit_test(test_size_valid), 
	cmocka_unit_test(test_print_invalid),
        cmocka_unit_test(test_print_valid), 
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
