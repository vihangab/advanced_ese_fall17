/*
	File name : circbuff.c
	File Description : Source files for Double Linked List
	File Author: Vihanga Bare
	Assignment Date : 9/18/2017
	Tools used : Ubuntu 16.04.2, GCC version 5.4.0	
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "circbuff.h"

/*
 Function - to check whether circular buffer is full
 */
cbuff_stat cbuffFull(struct CircBuff** circbuff)
{	
	if((*circbuff) == NULL)
	{
		return ERROR;
	}
	/*If count = length circular buffer is full*/
    if(((*circbuff)->count) == ((*circbuff)->length))
	{
        (*circbuff)->cbuff_status=BUFFER_FULL;
        
    }
    else
	{
		/*If count is less than length circular buffer is available*/
        ((*circbuff)->cbuff_status)=AVAILABLE;
    }
    
	/*Return circular buffer is full or available*/
    return ((*circbuff)->cbuff_status);
}

/*
Function - to check if buffer is empty
*/
cbuff_stat cbuffEmpty(struct CircBuff** circbuff)
{	
	/*invalid location*/
	if((*circbuff) == NULL)
		return ERROR;

	/*If Count=0 circular buffer is empty*/
	if((((*circbuff)->head)==((*circbuff)->tail)) && ((*circbuff)->count)==0)
	{
        ((*circbuff)->cbuff_status)=BUFFER_EMPTY;
    }
    else
	{
		/*If count!=0 buffer is available*/
        ((*circbuff)->cbuff_status)=AVAILABLE;
    }
	
    return ((*circbuff)->cbuff_status);
}

/*
Function -  used to add elements to circular buffer
*/
cbuff_stat cbuffAdd(struct CircBuff** circbuff,uint32_t data,uint32_t add_count)
{	

	/*Invalid loaction*/
	if((*circbuff) == NULL)
	{	
		return ERROR;
	}

	/*error if add count is zero*/
	if(add_count == 0)
	{
		
		return ERROR;
	}

	if(((*circbuff)->head == (*circbuff)->tail) && ((*circbuff)->count > ((*circbuff)->length)))
	{
		((*circbuff)->count)=0;	
	}

    while(add_count)
	{
		
        if(((*circbuff)->count)!=((*circbuff)->length)) //check if buffer is full
		{
			
            *((*circbuff)->head)=data;
            ((*circbuff)->head)++;
            ((*circbuff)->count)++;

			
            if((*circbuff)->head > ((*circbuff)->buffer+(*circbuff)->length-1)) //wrap around buffer
			{
                (*circbuff)->head=(*circbuff)->buffer;
            }
			/*Return available if buffer not full*/
            (*circbuff)->cbuff_status = AVAILABLE;
        }
        else
		{
			/*Return buffer full  length = count*/
            (*circbuff)->cbuff_status = BUFFER_FULL;
        }
        data++;
        add_count--;
    }
	
	return (*circbuff)->cbuff_status;
}

/*
Function - remove elements from circular buffer
*/
cbuff_stat cbuffRemove(struct CircBuff** circbuff,uint32_t* data, uint32_t remove_count)
{	
		
	/*Invalid location*/
	if((*circbuff) == NULL)
	{		
		return ERROR;
	}

	/*error if remove count is 0*/
	if(remove_count == 0)
	{
		
		return ERROR;
	}

    while(remove_count)
	{

		
        if(((*circbuff)->count)!=0)
		{
	
	     (*data) = *((*circbuff)->tail);
	     //printf("data is %d",*data);	
            *((*circbuff)->tail)=0;

			
	    
            ((*circbuff)->tail)++;  //traverse through buffer
            ((*circbuff)->count)--;

			
            if((*circbuff)->tail > ((*circbuff)->buffer + (*circbuff)->length-1)) //wrap around
			{
                (*circbuff)->tail=(*circbuff)->buffer;
            }
			
            (*circbuff)->cbuff_status = SUCCESS;
        }
        else
		{
			/* if buffer is empty, return buffer empty*/
            (*circbuff)->cbuff_status = BUFFER_EMPTY;
        }
        remove_count--;
    }
    
    return (*circbuff)->cbuff_status;
}

/*
Function - allocates heap memory to buffer
*/
cbuff_stat cbuffMalloc(struct CircBuff** circbuff, uint32_t buflen)
{	
	//allocate memory
	(*circbuff) = (struct CircBuff*) malloc(sizeof(struct CircBuff));
	if((*circbuff) == NULL)
	{
		(*circbuff)->cbuff_status = ERROR;
	}
	else
	{
		(*circbuff)->buffer = (uint32_t*) malloc(sizeof(uint32_t) * buflen);
		if((*circbuff)->buffer == NULL)
			(*circbuff)->cbuff_status = ERROR;
		else
			(*circbuff)->cbuff_status = AVAILABLE;
	}
	/*Return buffer pointer */
    return (*circbuff)->cbuff_status;
}

/*
Function - is used to free the heap memory allocate to buffer
*/
cbuff_stat cbuffDelete(struct CircBuff** circbuff)
{
	free((*circbuff)->buffer);
	free(*circbuff);
	
	(*circbuff)->buffer = NULL;
	*circbuff = NULL;

	/*After destroying memory return success*/
    return SUCCESS;
}


/*
Function - is used to initialize circular buffer, initialize count to 0*/
void cbuffInit(struct CircBuff** circbuff, uint32_t len)
{
	(*circbuff)->head=(*circbuff)->buffer;
	(*circbuff)->tail=(*circbuff)->buffer;
	(*circbuff)->count=0;
	(*circbuff)->length=len;
	
}

cbuff_stat cbuffPrint(struct CircBuff** circbuff)
{
	uint32_t i;
	
	if((*circbuff) == NULL)
		return ERROR;
	else
	{
		for(i= 0;i < (*circbuff)->count; i++)
		{
			//printf("Circular buffer : \n");
			//printf("%d \n", *((*circbuff)->tail++));
		}
	}
	return SUCCESS;
			
}

uint32_t sizeBuff(struct CircBuff** circbuff)
{
	if((*circbuff) == NULL)
		return 0;
	else
		return(*circbuff)->count;
}

void printStatus(cbuff_stat status)
{
	if(status == SUCCESS)
		printf(" success\n");
	else if(status == ERROR)
		printf(" ERROR\n");
	else if(status == MALLOC_FAILURE)
		printf(" MALLOC failure\n");
	else if(status == AVAILABLE)
		printf(" Available\n");
	else if(status == BUFFER_FULL)
		printf(" Buffer Full\n");
	else if(status == BUFFER_EMPTY)
		printf(" Buffer empty \n");
	else
		printf(" Unknown error \n");
}

/*int main(int argc, char **argv)
{
	//create pointer to head of Circular buffer
	struct CircBuff* head = NULL;
	cbuffMalloc(&head,BUFFLEN);
	cbuffInit(&head,BUFFLEN);
	
	
	uint32_t data[4] = {1,2,3,4};
	
	cbuff_stat  status = cbuffAdd(&head,data[0],1);
	printf("add to buffer value %d:",data[0]);
	printStatus(status);

	status  = cbuffAdd(&head,data[1],1);
	printf("add to buffer value %d:",data[1]);
	printStatus(status);	
	
	status  = cbuffAdd(&head,data[2],1);
	printf("add to buffer value %d:",data[2]);
	printStatus(status);
	
	status  = cbuffAdd(&head,data[3],1);
	printf("add to buffer value %d:",data[3]);
	printStatus(status);	
	
	cbuffPrint(&head);
	
	
	status  = cbuffRemove(&head,1);
	printf("remove value:");
	printStatus(status);

	cbuffPrint(&head);
	
	uint32_t size = sizeBuff(&head);
	printf("size of buffer %d:",size);
	printStatus(status);
	
	status  = cbuffFull(&head);
	printf("Buffer status:");
	printStatus(status);
	
	status  = cbuffRemove(&head,1);
	printf("remove value:");
	printStatus(status);
	
	status  = cbuffRemove(&head,1);
	printf("remove value:");
	printStatus(status);

	status  = cbuffRemove(&head,1);
	printf("remove value:");
	printStatus(status);	
	
	status  = cbuffEmpty(&head);
	printf("Buffer status:");
	printStatus(status);
	
	return 0;
}*/
