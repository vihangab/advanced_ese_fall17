/*
Author: Vihanga Bare
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//create Double linked list data structure
struct NodeDLL
{
	uint32_t data; //variable to insert data into linked list
	struct NodeDLL *next;
  	struct NodeDLL *prev;
};

/*
function name - insert_first(Node **head_ptr, uint32_t data);
purpose - take head pointer and new data and input and insert new data at the front of linked list
*/

void insertFirst(struct NodeDLL** head_ptr, uint32_t insert_data)
{
	//create a new element for DLL and allocate space for it using malloc
	struct NodeDLL* new_element = (struct NodeDLL*) malloc(sizeof(struct NodeDLL));
	//add new data into this new DLL node
	new_element->data = insert_data;
	//make the pointer to previous node point to NULL as this is the first element in the DLL
	new_element->prev = NULL;
	//make the pointer to next node point to head pointer as head pointer had the location of 		initial first element of linkedlist
	new_element->next = (*head_ptr);

  	//change the location of head pointer to make it point to this newly inserted element in DLL
  	if((*head_ptr) != NULL)
		(*head_ptr)->prev = new_element;
	(*head_ptr) = new_element;

}

/*
function name - print_DLL(struct NodeDLL *ptr_to_element);
purpose - print contents of DLL
*/
void printDLL(struct NodeDLL *ptr_to_element)
{
    struct NodeDLL *last;
    printf("\n DLL in forward direction \n");
    while (ptr_to_element != NULL)
    {
        printf(" %d		", ptr_to_element->data);
		printf("\n");
        last = ptr_to_element;
        ptr_to_element = ptr_to_element->next;
    }
 
    printf("\n DLL in reverse direction \n");
    while (last != NULL)
    {
        printf(" %d		", last->data);
		printf("\n");
        last = last->prev;
    }
	
}

/*
function name - insertNth(struct NodeDLL **head_ptr, uint32_t index, uint32_t data);
purpose - take head pointer, new data and index where data to be inserted in DLL
*/

void insertNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t data)
{
    
	/* if list in NULL or invalid position is given */
    if (*head_ptr == NULL || index <= 0)
        return;
	
	//dummy node
	struct NodeDLL* current_ptr = (*head_ptr);
	uint32_t i = 0;
	
	// got to the N th position of DLL from head ptr
	for(i = 1; (i < index) && (current_ptr != NULL) ; i++)
		current_ptr = current_ptr->next;
	
    if(index == 1)
		insertFirst(&(*head_ptr),index);
	
	if(current_ptr != NULL)
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
}

/*
function name - deleteNodeDLL(struct NodeDLL **head_ptr, NodeDLL* node_to_delete);
purpose - take head pointer, node to delete
*/

void deleteNodeDLL(struct NodeDLL** head_ptr, struct NodeDLL* node_to_delete)
{
	/* If node to be deleted is head node */
    if (*head_ptr == node_to_delete)
        *head_ptr = node_to_delete->next;
 
    /* Change next only if node to be deleted is NOT 
       the last node */
    if (node_to_delete)
        node_to_delete->next->prev = node_to_delete->prev;
 
    /* Change prev only if node to be deleted is NOT 
       the first node */
    if (node_to_delete)
        node_to_delete->prev->next = node_to_delete->next;
 
    /* Finally, free the memory occupied by del*/
    free(node_to_delete);
	
	printf("Node deleted successfully"); // to replace this with enum which returns status

	printDLL(*head_ptr);
}

/*
function name - deleteNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t data);
purpose - take head pointer, new data and index where data to be deleted in DLL
*/
void deleteNth(struct NodeDLL** head_ptr, uint32_t index, uint32_t data)
{
	/* if list in NULL or invalid position is given */
    if (*head_ptr == NULL || index <= 0)
        return;
 
    struct NodeDLL* current_ptr = *head_ptr;
    uint32_t i;
 
    /* traverse up to the node at position 'index' from
       the beginning */
    for (i = 1; current_ptr != NULL && i < index; i++)
        current_ptr = current_ptr->next;
 
    /* if 'index' is greater than the number of nodes
       in the doubly linked list */
    if (current_ptr == NULL)
        return;
 
    /* delete the node */
	deleteNodeDLL(head_ptr, current_ptr);
 
}

/* Function to delete the entire linked list */
void deleteDLL(struct NodeDLL** head_ptr)
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
}

/*
function name - search(struct NodeDLL** head_ptr, uint32_t index, uint32_t data);
purpose - take head pointer, new data and index where data to be deleted in DLL
*/
void searchDLL(struct NodeDLL** head_ptr, uint32_t data) 
{
   uint32_t index = 0;
   struct NodeDLL* current_ptr = (*head_ptr);
   
   if((*head_ptr) == NULL) 
   {
      printf("Linked List not initialized \n");
      return;
   } 

   current_ptr = (*head_ptr);
   
   while(current_ptr != NULL) 
   {
      index++;
      if(current_ptr->data == data) {
         printf("%d found at position %d\n", data, index);
         return;
      }
         
      if(current_ptr->next != NULL)
         current_ptr = current_ptr->next;
      else
         break;
   }
      
   printf("%d does not exist in the list\n", data);
}
/*
function name - sizeDLL(struct NodeDLL** head_ptr, uint32_t index, uint32_t data);
purpose - take head pointer, new data and index where data to be deleted in DLL
*/
void sizeDLL(struct NodeDLL** head_ptr) 
{
   uint32_t sizeDLL = 0;
   struct NodeDLL* current_ptr;
   
   if(head_ptr == NULL) {
      printf("\n Size of list is %d\n", sizeDLL);
      return;
   }

   current_ptr = (*head_ptr);
   
   
   while(current_ptr != NULL) {
      current_ptr = current_ptr->next;
      sizeDLL++;
   }

   printf("\n Size of list is %d\n", sizeDLL);
}

int main(int argc, char **argv)
{
	//create pointer to head of DLL
	struct NodeDLL* head = NULL;
	
	//insert an element at the beginning of the DLL
	insertFirst(&head,5);
	insertFirst(&head,4);
	insertFirst(&head,3);
	insertFirst(&head,2);
	insertFirst(&head,1);
	insertNth(&head,4,32);
	
	searchDLL(&head,32);
	sizeDLL(&head);
	deleteNth(&head,4,32);
	
	
	//print the elements of DLL
	printDLL(head);
	
	//Delete DLL
	deleteDLL(&head);
	if(head == NULL)
		printf("DLL deleted\n");
	
	//print the elements of DLL
	printDLL(head);
	return 0;
}








