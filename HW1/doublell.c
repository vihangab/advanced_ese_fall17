/*
	File name : doublell.c
	File Description : Source files for Double Linked List
	File Author: Vihanga Bare
	Assignment Date : 9/8/2017
	Tools used : Ubuntu 16.04.2, GCC version 5.4.0	
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "doublell.h"

/*
Function name - insert_first(Node **head_ptr, uint32_t data);
purpose - take head pointer and new data and input and insert new data at the front of linked list
*/

dll_stat insertFirst(struct NodeDLL** head_ptr, uint32_t insert_data)
{
	//create a new element for DLL and allocate space for it using malloc
	struct NodeDLL* new_element = (struct NodeDLL*) malloc(sizeof(struct NodeDLL));
	
	if(new_element == NULL)
		return MALLOC_FAILURE;
	
	//add new data into this new DLL node
	new_element->data = insert_data;
	
	//make the pointer to previous node point to NULL as this is the first element in the DLL
	new_element->prev = NULL;
	
	//make the pointer to next node point to head pointer as head pointer had the location of initial first element of linkedlist
	new_element->next = (*head_ptr);

  	//change the location of head pointer to make it point to this newly inserted element in DLL
  	if((*head_ptr) != NULL)
		(*head_ptr)->prev = new_element;
	(*head_ptr) = new_element;
	
	return SUCCESS;
}

/*
Function name - print_DLL(struct NodeDLL *ptr_to_element);
Purpose - print contents of DLL
*/
dll_stat printDLL(struct NodeDLL *ptr_to_element)
{
    //struct NodeDLL *last;
    printf("\n DLL in forward direction \n");
    while (ptr_to_element != NULL)
    {
        printf(" %d		", ptr_to_element->data);
		printf("\n");
        //last = ptr_to_element;
        ptr_to_element = ptr_to_element->next;
    }
 
    /*printf("\n DLL in reverse direction \n");
	
    while (last != NULL)
    {
        printf(" %d		", last->data);
		printf("\n");
        last = last->prev;
    }	
	
	*/
	return SUCCESS;
}

/*
Function name - insertNth(struct NodeDLL **head_ptr, uint32_t index, uint32_t data);
Purpose - take head pointer, new data and index where data to be inserted in DLL
*/

dll_stat insertNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t data)
{
    
	/* if trying to insert at an index which is greater than
	size of DLL */
    if (index > sizeDLL(head_ptr)+1)
        return INVALID_INDEX;
	
	//dummy node
	struct NodeDLL* current_ptr = (*head_ptr);
	//struct NodeDLL* last = (*head_ptr);
	
	uint32_t i = 0;
	
	// got to the N th position of DLL from head ptr
	for(i = 1; (i < index) && (current_ptr->next != NULL) ; i++)
	{
		current_ptr = current_ptr->next;
	}
	
    if(index == 1 || (*head_ptr) == NULL)
	{
		//if(index == 1 && (*head_ptr) == NULL)
			insertFirst(&(*head_ptr),index);
	}
	else if(current_ptr->next == NULL) //if pointer to last node
	{ 		
			struct NodeDLL* new_element = (struct NodeDLL*) malloc(sizeof(struct NodeDLL));
			if(new_element == NULL)
			{
				printf("unable to allocate memory");
				return ERROR;
			}
			new_element->data = data;
			
			//make the pointer to NEXT node point to NULL as this is the LAST element in the DLL
			new_element->next = NULL;
			
			//make the pointer to previous node point to the location of initial last element of DLL
			/* Change the next of last node */
				current_ptr->next = new_element;
 
			/* Make last node as previous of new node */
				new_element->prev = current_ptr;
			
			//printf("Node added successfully to end of list");
			
			return SUCCESS;		
	}
	else if(current_ptr != NULL)
	{
		//create a new element for DLL and allocate space for it using malloc
		struct NodeDLL* new_element = (struct NodeDLL*) malloc(sizeof(struct NodeDLL));
		
		//add new data into this new DLL node
		new_element->data = data;
		
		//make the pointer to next node point to head pointer as head pointer had the location of 		initial first element of linkedlist
		new_element->next = current_ptr->next;

		//make the pointer to previous node point to NULL as this is the first element in the DLL
		new_element->prev = current_ptr;
		
		if(current_ptr->next != NULL)
			current_ptr->next->prev = new_element;
		current_ptr->next = new_element;
		
		printf("Node inserted successfully"); // to replace this with enum which returns status
		
		printDLL(*head_ptr);
	}
	else
		return ERROR;
	
	return SUCCESS;
}

/*
function name - deleteNodeDLL(struct NodeDLL **head_ptr, NodeDLL* node_to_delete,uint32_t* data);
purpose - take head pointer, node to delete
*/

dll_stat deleteNodeDLL(struct NodeDLL** head_ptr, struct NodeDLL* node_to_delete, uint32_t* data)
{
	/* base case */
	if(*head_ptr == NULL || node_to_delete == NULL)
		return ERROR;
	
	*data = node_to_delete->data; //tale data from the node
	
	/* If node to be deleted is head node */
    if (*head_ptr == node_to_delete)
        *head_ptr = node_to_delete->next;
	
 
    /* Change next only if node to be deleted is NOT 
       the last node */
    if (node_to_delete->next != NULL)
        node_to_delete->next->prev = node_to_delete->prev;
 
    /* Change prev only if node to be deleted is NOT 
       the first node */
    if (node_to_delete->prev != NULL)
        node_to_delete->prev->next = node_to_delete->next;
 
    /* Finally, free the memory occupied by del*/
    free(node_to_delete);
	
	return SUCCESS;
}

/*
function name - deleteNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t* data);
purpose - take head pointer, new data and index where data to be deleted in DLL
*/
dll_stat deleteNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t* data)
{
	/* if trying to insert at an index which is greater than
	size of DLL */
    if (index > sizeDLL(head_ptr))
        return INVALID_INDEX;

    struct NodeDLL* current_ptr = *head_ptr;
    uint32_t i;
 
    /* traverse up to the node at position 'index' from
       the beginning */
    for (i = 1; current_ptr != NULL && i < index; i++)
        current_ptr = current_ptr->next;
	
	if(index == 1)
	{
		if((*head_ptr) ==NULL)
			return ERROR;
		else
		{
			(*data) = current_ptr->data;   //take data from node to be removed
			(*head_ptr) = (*head_ptr)->next;
			(*head_ptr)->prev = NULL;
			free(current_ptr);
			printf("Deleted node from beginning successfully");	
		}
	}
	else if (current_ptr->next == NULL) //current ptr points to last node
	{
		if(current_ptr == NULL)
			return ERROR;     //return error as cannot delete from empty list
		struct NodeDLL* del = current_ptr;
		
		(*data) = del->data;   //take data from node to be removed
		current_ptr =  current_ptr->prev;
		current_ptr->next = NULL;
		free(del);
		printf("Deleted node from end successfully");	
	}
	else if (current_ptr != NULL)
	{
		/* delete the node */
		deleteNodeDLL(head_ptr, current_ptr,data);
	}
	else
	{
		printf("invalid position");
		return ERROR;
	}
	return SUCCESS;
}

/* Function to delete the entire linked list */
dll_stat deleteDLL(struct NodeDLL** head_ptr)
{
   /* deref head_ptr to get the real head */
   struct NodeDLL* current_ptr = *head_ptr;
   struct NodeDLL* next_ptr;
 
   while (current_ptr != NULL) 
   {
       next_ptr = current_ptr->next;
       free(current_ptr);
       current_ptr = next_ptr;
   }
   
   /* deref head_ptr to affect the real head back
      in the caller. */
   *head_ptr = NULL;
   
   return SUCCESS;
}

/*
function name - search(struct NodeDLL** head_ptr, uint32_t index, uint32_t data);
purpose - take head pointer, new data and index where data to be deleted in DLL
*/
dll_stat searchDLL(struct NodeDLL** head_ptr, uint32_t data, uint32_t* index) 
{
   *(index)= 0;
   uint32_t count = *(index);
   struct NodeDLL* current_ptr = (*head_ptr);
   
   if((*head_ptr) == NULL) 
   {
      //printf("Linked List not initialized \n");
      return ERROR;
   } 

   current_ptr = (*head_ptr);
   
   while(current_ptr != NULL) 
   {
      count++;
      if(current_ptr->data == data) {
         //printf("%d found at position %d\n", data, index);
         return ERROR;
      }
         
      if(current_ptr->next != NULL)
         current_ptr = current_ptr->next;
      else
         break;
   }
   *(index) = count;
      
   //printf("%d does not exist in the list\n", data);
   
   return SUCCESS;
}
/*
function name - sizeDLL(struct NodeDLL** head_ptr, uint32_t index, uint32_t data);
purpose - take head pointer, new data and index where data to be deleted in DLL
*/
uint32_t sizeDLL(struct NodeDLL** head_ptr) 
{
   uint32_t sizeDLL = 0;
   struct NodeDLL* current_ptr;
   
   if(head_ptr == NULL) {
      //printf("\n Size of list is %d\n", sizeDLL);
      return 0;
   }

   current_ptr = (*head_ptr);
   
   
   while(current_ptr != NULL) {
      current_ptr = current_ptr->next;
      sizeDLL++;
   }

   //printf("\n Size of list is %d\n", sizeDLL);
   
   return sizeDLL;
}

void printStatus(dll_stat status)
{
	if(status == SUCCESS)
		printf(" success\n");
	else if(status == ERROR)
		printf(" ERROR\n");
	else if(status == MALLOC_FAILURE)
		printf(" MALLOC failure\n");
	else if(status == INVALID_INDEX)
		printf(" Invalid Index\n");
	else
		printf(" Unknown Error \n");
}

int main(int argc, char **argv)
{
	//create pointer to head of DLL
	struct NodeDLL* head = NULL;
	uint32_t data,position;
	uint32_t ip_ptr;
	
	data = 1;
	position = 1;
	
	dll_stat status = insertNth(&head,position,data);
	printf("Insert at %d:",position);
	printStatus(status);
	
	data = 2;
	position = 2;
	
	status = insertNth(&head,position,data);
	printf("Insert at %d:",position);
	printStatus(status);
	
	data = 3;
	position = 3;
	
	status = insertNth(&head,position,data);
	printf("Insert at %d:",position);
	printStatus(status);
	
	data = 22;
	position = 2;
	
	status = insertNth(&head,position,data);
	printf("Insert at %d:",position);
	printStatus(status);
	
	status = printDLL(head);
	printf("Print DLL : ");
	printStatus(status);

	position = 2;

	status = deleteNth(&head,position,&ip_ptr);
	printf("Deleted at %d position, value returned %d: ",position,ip_ptr);
	
	position = 3;

	status = deleteNth(&head,position,&ip_ptr);
	printf("Deleted at %d position, value returned %d: ",position,ip_ptr);
	
	status = printDLL(head);
	printf("Print DLL : ");
	printStatus(status);
	
	ip_ptr = sizeDLL(&head);
	printf("Size of DLL is %d: ",ip_ptr);
	data = 22;
	
	status = searchDLL(&head,data,&ip_ptr);
	printf("data %d found at %d: ",data,ip_ptr);
	printStatus(status);
	
	status = deleteDLL(&head);
	if(head == NULL)
	{
		printf("DLL deleted :");
		printStatus(status);
	}
	return 0;
}








