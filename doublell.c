/*Author: Vihanga Bare
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
  //make the pointer to next node point to head pointer as head pointer had the location of initial first element of linkedlist
  new_element->next = (*head_ptr);

  //change the location of head pointer to make it point to this newly inserted element in DLL
  if((*head_ptr) != NULL)
   {

   	(*head_ptr)->prev = new_element;

   }
   (*head_ptr) = new_element;

}

/*
function name - print_DLL(Node **head_ptr);
purpose - print contents of DLL
*/
void printDLL(struct NodeDLL *ptr_to_element)
{
  while (ptr_to_element != NULL)
  {
     printf(" %d \n", ptr_to_element->data);
     ptr_to_element = ptr_to_element->next;
  }
}

int main(int argc, char **argv)
{
  //create pointer to head of DLL
  struct NodeDLL* head = NULL;
  //insert an element at the beginning of the DLL
  insertFirst(&head,5);
  //print the elements of DLL
  printDLL(head);
  
  return 0;
}








