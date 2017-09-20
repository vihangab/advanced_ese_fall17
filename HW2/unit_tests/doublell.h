/*
	File name : doublell.h
	File Description : Header files for Double Linked List
	File Author: Vihanga Bare
	Assignment Date : 9/8/2017
	Tools used : Ubuntu 16.04.2, GCC version 5.4.0	
*/

#ifndef DOUBLELL_H_
#define DOUBLELL_H_

/****Enum used for returning the DLL state****/
typedef enum dll_stat
{
	SUCCESS,
	ERROR,
	MALLOC_FAILURE,
	INVALID_INDEX,
}dll_stat;

/****Double Linked List structure*****/

struct NodeDLL
{
	uint32_t data; //variable to insert data into linked list
	struct NodeDLL *next;
  	struct NodeDLL *prev;
};

/*
Function name - dll_stat insertFirst(struct NodeDLL** head_ptr, uint32_t insert_data);
Purpose - Insert a node at beginning of DLL
Input - NodeDLL **head_ptr is pointer to start of DLL and insert_data is data to be inserted
Return Values - Returns status as enum
*/
dll_stat insertFirst(struct NodeDLL** head_ptr, uint32_t insert_data);


/*
Function name - dll_stat printDLL(struct NodeDLL* ptr_to_element);
Purpose - Print contents of DLL
Input - NodeDLL *ptr_to_element is pointer to start of DLL
Return Values - Returns status as enum
*/
dll_stat printDLL(struct NodeDLL* ptr_to_element);


/*
Function name - dll_stat insertNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t data);
Purpose - Insert element at Nth position in DLL 
Input - struct NodeDLL** head_ptr is pointer to head, uint32_t index in position at which to insert, uint32_t data is data to be inserted
Return Values - Returns status as enum
*/
dll_stat insertNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t data);

/*
Function name - dll_stat deleteNodeDLL(struct NodeDLL** head_ptr, struct NodeDLL* node_to_delete);
Purpose - Delete node in DLL 
Input - struct NodeDLL** head_ptr is pointer to head, struct NodeDLL* node_to_delete is pointer to node to detele
Return Values - Returns status as enum
*/
dll_stat deleteNodeDLL(struct NodeDLL** head_ptr, struct NodeDLL* node_to_delete,uint32_t* data);

/*
Function name - dll_stat deleteNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t data);
Purpose - Delete node at Nth position in DLL 
Input - struct NodeDLL** head_ptr is pointer to head, uint32_t index is position of node to dleete, uint32_t* data returns pointer to data deleted
Return Values - Returns status as enum
*/
dll_stat deleteNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t* data);

/*
Function name - dll_stat (struct NodeDLL** head_ptr);
Purpose - Delete entire DLL 
Input - struct NodeDLL** head_ptr is pointer to head
Return Values - Returns status as enum
*/
dll_stat deleteDLL(struct NodeDLL** head_ptr);

/*
Function name - dll_stat searchDLL(struct NodeDLL** head_ptr, uint32_t data,uint32_t* index);
Purpose - search first occurence of an element in DLL
Input - struct NodeDLL** head_ptr is pointer to head, uint32_t data to be searched, index will return the index at which data was found
Return Values - Returns status as enum
*/
dll_stat searchDLL(struct NodeDLL** head_ptr, uint32_t data,uint32_t* index);

/*
Function name - uint32_t sizeDLL(struct NodeDLL** head_ptr);
Purpose - returns size of DLL
Input - struct NodeDLL** head_ptr is pointer to head
Return Values - Returns integer value for size of DLL
*/
uint32_t sizeDLL(struct NodeDLL** head_ptr);

/*
Function name - void printStatus(dll_state status);
Purpose - prints status messages
Input - takes enum as input
Return Values - no return value
*/
void printStatus(dll_stat status);

#endif /* DOUBLELL_H_ */
