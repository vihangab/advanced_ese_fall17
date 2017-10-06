/******************************************************************************
* File: Vihanga Bare
  Description: Source code examples to demo use of following pthread functions - 
mutex_init
mutex_destroy
mutex_lock
mutex_trylock
mutex_unlock
cond_init
cond_signal
cond_wait
cond_destroy 
File Author: Vihanga Bare
Assignment Date : 10/04/2017
Tools used : Ubuntu 16.04.4, GCC version 5.4.0

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 3		/* Size of shared buffer */
#define ITERATIONS	5  /* Iterations define the count of numbers inserted into the buffer */

int buffer[BUF_SIZE];  	/* shared buffer */
int add = 0;  			/* place to add next element */
int rem = 0;  			/* place to remove next element */
int num = 0;  			/* number elements in buffer */
int end = 0;  			/* global counter to forcefully end the program */

pthread_mutex_t mutex;  	/* mutex lock for buffer */
pthread_cond_t condvar_consumer; /* consumer waits on this condition variable */
pthread_cond_t condvar_producer; /* producer waits on this cond variable */

/*
Function - void *producer (void *param);
Description - Producer Thread inserts values into the shared buffer of size BUF_SIZE, buffer wraps around after BUF_SIZE has reached. 
If buffer is full, waits on a signal from the from consumer thread using pthread_cond_wait, that values have been removed from buffer 
and buffer is available again to insert values.
*/
void *producer (void *param);

/*
Function - void *consumer (void *param);
Description - Consumer Thread removes values from the shared buffer of size BUF_SIZE, buffer wraps around after BUF_SIZE has reached. 
If buffer is empty, waits on a signal from the from producer thread using pthread_cond_wait, that values have been added from buffer 
and buffer is available again to remove values. As producer thread, inserts only 'ITERATIONS' values into the buffer, same value in 
a global variable 'end' has been used to end the continuous while() loop in consumer thread.
*/
void *consumer (void *param);

/*
Function - int pthread_mutex_destroy(pthread_mutex_t *mutex);
			int pthread_mutex_init(pthread_mutex_t *restrict mutex,	const pthread_mutexattr_t *restrict attr);
Description - The pthread_mutex_destroy() function shall destroy the mutex object referenced by mutex; 
				The pthread_mutex_init() function shall initialize the mutex referenced by mutex with attributes specified by attr. 
				If attr is NULL, the default mutex attributes are used; 

Function - void __sched mutex_lock (struct mutex * lock); 
Description - Lock the mutex exclusively for this task. 
				If the mutex is not available right now, it will sleep until it can get it.
				The mutex must later on be released by the same task that acquired it. 
				Recursive locking is not allowed. The task may not exit without first unlocking the mutex. 
				Also, kernel memory where the mutex resides must not be freed with the mutex still locked. 
				The mutex must first be initialized (or statically defined) before it can be locked. memset-ing the mutex to 0 is not allowed.

Function - void __sched mutex_unlock (struct mutex * lock);
Description - Unlock a mutex that has been locked by this task previously. This function must not be used in interrupt context. 
				Unlocking of a not locked mutex is not allowed. This function is similar to (but not equivalent to) up.

Function - int __sched mutex_trylock (	struct mutex * lock);
Description - Try to acquire the mutex atomically. Returns 1 if the mutex has been acquired successfully, and 0 on contention.

Function - int cond_init(cond_t *cvp, int type, void *arg); int cond_wait(cond_t *cvp, mutex_t *mp); int cond_signal(cond_t *cvp); 
			int cond_broadcast(cond_t *cvp); int cond_destroy(cond_t *cvp);
			
Description - cond_init() initializes the condition variable pointed to by cvp . A condition variable types - USYNC_THREAD, USYNC_PROCESS
The condition wait interface allows a thread to wait for a condition and atomically release the associated mutex that it needs to hold to 
check the condition. The thread waits for another thread to make the condition true and that thread's resulting call to signal and wakeup 
the waiting thread. cond_wait() atomically releases the mutex pointed to by mp and causes the calling thread to block on the condition variable 
pointed to by cvp .
A condition signal allows a thread to unblock the next thread waiting on the condition variable, whereas, a condition broadcast allows a 
thread to unblock all threads waiting on the condition variable.
cond_signal() unblocks one thread that is blocked on the condition variable pointed to by cvp .
cond_broadcast() unblocks all threads that are blocked on the condition variable pointed to by cvp .
The condition destroy functions destroy any state, but not the space, associated with the condition variable.
cond_destroy() destroys any state associated with the condition variable pointed to by cvp . The space for storing the condition variable 
is not freed.
*/

int main(int argc, char *argv[])
{

	pthread_t t1, t2, t3;
	int retval;				//return values to check error cases
	
	pthread_mutex_init(&mutex, NULL); 			// initialise your mutex, and condition variables
	if(retval != 0)
	{
		printf("mutex initialisation failed, error code - %d\n", retval);
	}
	pthread_cond_init(&condvar_consumer, NULL);
	if(retval != 0)
	{
		printf("consumer condition variable initialisation failed, error code - %d\n", retval);
	}

	pthread_cond_init(&condvar_producer, NULL);
	if(retval != 0)
	{
		printf("producer condition variable initialisation failed, error code - %d\n", retval);
	}
	
	//create threads

	if(pthread_create(&t1, NULL, producer, NULL) != 0) 
	{
		printf("Unable to create producer thread\n");
		return 0;
	}
	
	if(pthread_create(&t2, NULL, producer, NULL) != 0) 
	{
		printf("Unable to create consumer thread\n");
		return 0;
	}
	
	if(pthread_create(&t3, NULL, consumer, NULL) != 0) 
	{
		printf("Unable to create consumer thread\n");
		return 0;
	}

	/* wait for created threads to exit */
	if(pthread_join(t1, NULL) != 0)
	{
	  printf("ERROR; return code from pthread_join() is - %d\n", retval);
	}
	
	if(pthread_join(t2, NULL) != 0)
	{
	  printf("ERROR; return code from pthread_join() is - %d\n", retval);
	}
	
	if(pthread_join(t3, NULL) != 0)
	{
	  printf("ERROR; return code from pthread_join() is - %d\n", retval);
	}
	
	printf("Parent quiting\n");

	//destroy mutex and condition variables
	if(pthread_mutex_destroy(&mutex) != 0)
	{
	  printf("mutex destroy failed, error code - %d\n", retval);
	}
	
	if(pthread_cond_destroy(&condvar_consumer) != 0)
	{
	  printf("consumer condition var destroy failed, error code - %d\n", retval);
	}
	
	if(pthread_cond_destroy(&condvar_producer) != 0)
	{
	  printf("consumer condition var destroy failed, error code - %d\n", retval);
	}
	
	return 0;
}

/* Producer function */
void *producer(void *param) 
{

	int i,ret;
	for (i=1; i<=ITERATIONS; i++) 
	{
		if(end > ITERATIONS)
		{
			printf("producer exit as no. of iterations exhausted........\n");
			pthread_exit(NULL);
		}
		/* Insert into buffer */
		ret = pthread_mutex_lock(&mutex);
		if(ret != 0)
		{
		  printf("mutex lock , error code - %d\n", ret);
		}
		
		if (num > BUF_SIZE) 
		{
			printf ("num > BUFSIZE, num value is :%d\n", num);
			exit(1);  /* overflow */
		}

		while (num == BUF_SIZE) 
		{  /* wait on a condition from consumer thread if buffer is full */
			printf ("buffer is full, num value is :%d\n", num);
			
			ret = pthread_cond_wait(&condvar_producer, &mutex);
			if(ret != 0)
			{
			  printf("condition wait , error code - %d\n", ret);
			}
		}
		
		/* buffer not full so add element */
		buffer[add] = i;
		printf ("value inserted into buffer is :%d\n", i);
		add = (add+1) % BUF_SIZE;
		num++;
		ret = pthread_mutex_unlock(&mutex);
		if(ret != 0)
		{
		  printf("mutex unlock , error code - %d\n", ret);
		}
		
		/* signal consumer thread that values have been inserted into buffer*/
		ret = pthread_cond_signal(&condvar_consumer);
		if(ret != 0)
		{
		  printf("condition signal , error code - %d\n", ret);
		}
		printf ("producer: inserted %d\n", i);
	}

	printf("producer exit........\n");
	return 0;
}

/*Consumer Function */
void *consumer(void *param) 
{
	int i, ret;
	while(1) 
	{
		end++;
		if(end > ITERATIONS)
		{
			printf ("Consume value %d\n", i);
			printf("Consumer exit as no. of iterations exhausted........\n");
			pthread_exit(NULL);
		}
		ret = pthread_mutex_trylock(&mutex);
		if(ret != 0)
		{
		  printf("mutex try lock , error code - %d\n", ret);
		}
		if (num < 0) 
		{
			printf ("num < 0, num value is %d\n", num);
			exit(1);
		} /* underflow */

		while (num == 0) 
		{  /* wait on a signal from producer thread if buffer empty */
			printf ("buffer is empty, num value is :%d\n", num);
			printf ("about to wait for conditon from producer\n");
			ret = pthread_cond_wait(&condvar_consumer, &mutex);
			if(ret != 0)
			{
			  printf("mutex try lock , error code - %d\n", ret);
			}
		}

		/* buffer not empty so remove element */
		i = buffer[rem];
		printf ("value removed from buffer is :%d\n", i);
		rem = (rem+1) % BUF_SIZE;
		num--;
		ret = pthread_mutex_unlock(&mutex);
		if(ret != 0)
		{
		  printf("mutex unlock , error code - %d\n", ret);
		}
		
		/*signal producer thread that values have been removed and buffer is available to insert values*/
		ret = pthread_cond_signal(&condvar_producer);
		if(ret != 0)
		{
		  printf("mutex unlock , error code - %d\n", ret);
		}
		printf ("Consume value %d\n", i);  
	}
	return 0;
}