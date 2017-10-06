/******************************************************************************
File: Vihanga Bare
Description: Source code examples to demo use of following pthread functions -  
create
join
exit
self
getattr
getattr_default
setattr 
File Author: Vihanga Bare
Assignment Date : 10/04/2017
Tools used : Ubuntu 16.04.4, GCC version 5.4.0
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define NUM_THREADS	10

char** messages = NULL;

/*
Function - *printMsg(void *threadid);
Description - takes void pointer *threadid as argument, which contains value for thread ID, prints the thread ID 

Function - pthread_t pthread_self(void);
Description - pthread_self - obtain ID of the calling thread. To check the output of pthread_self() I am printing the thread ID 
				of created thread stored in pthread_t threads[t] 

Function - void pthread_exit(void *retval);
Description - The pthread_exit() function terminates the calling thread and returns
				a value via retval that (if the thread is joinable) is available to
				another thread in the same process that call pthread_join() on this thread.
*/
void *printMsg(void *threadid)
{
   long taskid;
   taskid = (long) threadid;
   printf("Thread %ld: %s\n", taskid,messages[taskid]);
   printf("Result of pthread_self inside printMsg function: %ld \n",pthread_self());
   printf("Exiting printMsg routine..........\n");
   
   pthread_exit((void*) threadid);
}

/*
Function - int pthread_attr_init(pthread_attr_t *attr); int pthread_attr_destroy(pthread_attr_t *attr);
Description - The pthread_attr_init() function initializes the thread attributes object pointed to by 'attr' with default attribute values.
				When a thread attributes object is no longer required, it should be destroyed using the pthread_attr_destroy() function. 
				
Function - int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
Description - The pthread_create() function starts a new thread in the calling process.  The new thread starts execution by invoking start_routine(); 
				arg is passed as the sole argument of start_routine().

				
Function - int pthread_attr_getstacksize(const pthread_attr_t *restrict attr, size_t *restrict stacksize);
			int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
Description - The pthread_attr_getstacksize() and pthread_attr_setstacksize() functions, respectively, shall get and set the thread creation 
				stacksize attribute in the attr object. The stacksize attribute shall define the minimum stack size (in bytes) allocated for 
				the created threads stack. Upon successful completion, pthread_attr_getstacksize() and pthread_attr_setstacksize() shall 
				return a value of 0; otherwise, an error number shall be returned to indicate the error.The pthread_attr_getstacksize() function 
				stores the stacksize attribute value in stacksize if successful.
				
Function - int pthread_attr_getdetachstate(const pthread_attr_t *attr,int *detachstate); 
			int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);				
Description - The detachstate attribute controls whether the thread is created in a detached state. The pthread_attr_getdetachstate() and 
				pthread_attr_setdetachstate() functions, respectively, shall get and set the detachstate attribute in the attr object.
				For pthread_attr_getdetachstate(), detachstate shall be set to either PTHREAD_CREATE_DETACHED or PTHREAD_CREATE_JOINABLE.
				For pthread_attr_setdetachstate(), the application shall set detachstate to either PTHREAD_CREATE_DETACHED or PTHREAD_CREATE_JOINABLE.
				
Function - int pthread_attr_setguardsize(pthread_attr_t *attrsize_t" guardsize ); 
			int pthread_attr_getguardsize(pthread_attr_t *attrsize_t *" guardsize );
Description  - The pthread_attr_setguardsize() function sets the guard size attribute of the thread attributes object referred to by attr to the 
				value specified in guardsize. If guardsize is greater than 0, then for each new thread created using attr the system allocates an 
				additional region of at least guardsize bytes at the end of the thread's stack to act as the guard area for the stack.
				If guardsize is 0, then new threads created with attr will not have a guard area.
				The default guard size is the same as the system page size.
*/

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	long taskids[NUM_THREADS]; 
	int32_t err_num, t, i;
	void *status;
	size_t stacksize;
	size_t guardsize;
	int32_t thread_state;
	pthread_t thread_ID;
	int32_t retval;
	
	
	pthread_attr_t attr;  //pthread attribute variable which contains default threa attributes for initialisation
	retval = pthread_attr_init(&attr); //initialise to default attribute values
	if(retval != 0)
	{
		printf("pthread attributes initialised failed, error code - ! %s\n", strerror(errno));
	}		
	messages = (char**)calloc(NUM_THREADS, sizeof(char*)); //allocate space for each string
	if(!messages)
	{
		printf("Calloc failed\n");
		exit(-1);
	}
	
    for(i = 0; i < NUM_THREADS; i++)
	{
        //printf("%d\n", i);
        messages[i] = (char*)calloc(50, sizeof(char));
		if(!messages)
		{
			printf("Calloc failed\n");
			exit(-1);
		}	
    }
	
	for(i=0;i < NUM_THREADS;i++)
	{
		sprintf(messages[i],"Msg %d",i);
	}
	  

	for(t=0;t<NUM_THREADS;t++) 
	{
		taskids[t] = t;

		retval = pthread_attr_getstacksize(&attr, &stacksize);
		if(retval != 0)
		{
			printf("failed to get stack size, error code - %d\n", retval);
		}
		printf("Thread %ld: stack size before = %li bytes \n", taskids[t], stacksize);

		pthread_attr_getdetachstate(&attr, &thread_state);
		if(retval != 0)
		{
			printf("failed to get detach state, error code - %d\n", retval);
		}
		printf("Thread %ld: state before = %d\n", taskids[t], thread_state);

		pthread_attr_getguardsize(&attr, &guardsize);
		if(retval != 0)
		{
			printf("failed to get guard size, error code - %d\n", retval);
		}
		printf("Thread %ld: guard size before = %li bytes \n", taskids[t], guardsize);

		
		//set your own values	for pthread attribute variables
		stacksize = NUM_THREADS * 50 * 20 * sizeof(char);		//as it is an array of strings this means, 20 characters per string and 50 such strings  
		guardsize = NUM_THREADS * 50 * 50 * sizeof(char);
		thread_state = PTHREAD_CREATE_JOINABLE;
		pthread_attr_setstacksize(&attr, stacksize);
		if(retval != 0)
		{
			printf("failed to set stack size, error code - %d\n", retval);
		}
		printf("Thread %ld: stack size after = %li bytes \n", taskids[t], stacksize);

		pthread_attr_setdetachstate(&attr, thread_state);
		if(retval != 0)
		{
			printf("failed to set detach state, error code - %d\n", retval);
		}
		printf("Thread %ld: state after = %d\n", taskids[t], thread_state);

		pthread_attr_setguardsize(&attr, guardsize);
		if(retval != 0)
		{
			printf("failed to set guard size, error code - - %d\n", retval);
		}
		printf("Thread %ld: guard size after = %li bytes \n", taskids[t], guardsize);

		err_num = pthread_create(&threads[t], &attr, printMsg, (void *) taskids[t]);
		if (err_num) 
		{  
			printf("ERROR; return code from pthread_create() is - %d\n", err_num);
		}
		printf("pthread_create pthread_t Thread ID: %ld \n",threads[t]);
	
	}
		pthread_attr_destroy(&attr); //destroy set pthread attributes in attr variable
		if (retval != 0) 
		{  
			printf("pthread atribute destroy function failed, error code - is - %d\n", retval);
		}
		
		for(t=0; t<NUM_THREADS; t++) 
		{
			err_num = pthread_join(threads[t], &status);
			if (err_num) 
			{
			  printf("ERROR; return code from pthread_join() is %d\n", err_num);
			  exit(-1);
			}
			printf("Main: completed join with thread %d having a status of %ld\n",t,(long)status);
		}

		printf("Exiting main thread..........\n");
		pthread_exit(NULL);
}
