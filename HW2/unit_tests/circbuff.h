/*
	File name : circbuff.h
	File Description : Source files for Double Linked List
	File Author: Vihanga Bare
	Assignment Date : 9/8/2017
	Reference Link : https://github.com/vihangab/IoT_Project.git
	Tools used : Ubuntu 16.04.2, GCC version 5.4.0	
*/

#include <stdint.h>

#ifndef CIRCBUFF_H_
#define CIRCBUFF_H_

#define BUFFLEN 4

/*Enum used for returning the buffer state*/
typedef enum cbuff_stat
{
	SUCCESS,
	ERROR,
	BUFFER_FULL,
    	BUFFER_EMPTY,
	MALLOC_FAILURE,
    AVAILABLE
}cbuff_stat;

/*Circular buffer structure*/
struct CircBuff
{
     uint32_t* buffer;
     uint32_t* head;
     uint32_t* tail;
     uint32_t length;
     uint32_t count;
     cbuff_stat cbuff_status;
};

/*
 Function - to check whether circular buffer is full
 */
cbuff_stat cbuffFull(struct CircBuff** circbuff);


/*
 Function - to check whether circular buffer is empty
 */
cbuff_stat cbuffEmpty(struct CircBuff** circbuff);

/*
 Function name - cbuff_stat cbuffAdd(struct CircBuff** circbuff,uint32_t data,uint32_t add_count);
 Purpose - Add values ot circular buffer
 Input - struct CircBuff** circbuff pointer to circ buff ,uint32_t data - data to be inserted ,uint32_t add_count - number of values
 Return Values - Returns status as enum
*/
cbuff_stat cbuffAdd(struct CircBuff** circbuff,uint32_t data,uint32_t add_count);


/*
 Function name - cbuff_stat cbuffRemove(struct CircBuff** circbuff,uint32_t remove_count);
 Purpose - Remove values from circular buffer
 Input - struct CircBuff** circbuff pointer to circ buff ,uint32_t remove_count - number of values
 Return Values - Returns status as enum
*/
cbuff_stat cbuffRemove(struct CircBuff** circbuff,uint32_t* data, uint32_t remove_count);


/*
 Function name - cbuff_stat  cbuffMalloc(struct CircBuff** circbuff, uint32_t buflen);
 Purpose - allocate memory for circular buffer
 Input - struct CircBuff** circbuff pointer to circular buffer , uint32_t buflen - length of buffer
 Return Values - Returns status as enum
*/
cbuff_stat  cbuffMalloc(struct CircBuff** circbuff, uint32_t buflen);


/*
 Function name - cbuff_stat cbuffDestroy(struct CircBuff** circbuff);
 Purpose - allocate memory for circular buffer
 Input - struct CircBuff** circbuff pointer to circular buffer , uint32_t buflen - length of buffer
 Return Values - Returns status as enum
*/
cbuff_stat cbuffDelete(struct CircBuff** circbuff);


/*
 Function name - void cbuffInit(struct CircBuff** circbuff, uint32_t len);
 Purpose - Initialise circular buffer
 Input - struct CircBuff** circbuff pointer to circular buffer , uint32_t buflen - length of buffer
 Return Values - No Return Value
*/
void cbuffInit(struct CircBuff** circbuff, uint32_t len);

/*
 Function name - cbuff_stat cbuffPrint(struct CircBuff** circbuff);
 Purpose - print circular buffer
 Input - struct CircBuff** circbuff pointer to circular buffer
 Return Values - Returns status as enum
*/
cbuff_stat cbuffPrint(struct CircBuff** circbuff);

/*
Function name - void printStatus(cbuff_stat status);
Purpose - prints status messages
Input - takes enum as input
Return Values - no return value
*/
void printStatus(cbuff_stat status);

/*
Function name - void printStatus(cbuff_stat status);
Purpose - prints status messages
Input - takes enum as input
Return Values - no return value
*/
uint32_t sizeBuff(struct CircBuff** circbuff);


#endif /* CIRCBUFF_H_ */
