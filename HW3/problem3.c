/******************************************************************************
  File: Vihanga Bare
  Description: Source code examples to demo use of fork, exec, pthread_create, and profile cpu time required for each - 
  File Author: Vihanga Bare
  Assignment Date : 10/04/2017
  Tools used : Ubuntu 16.04.4, GCC version 5.4.0
******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "mylib.h"
#define NUM_THREADS	1

long taskid;

/*
Function - *printMsg(void *threadstart);
Description - takes void pointer *threadid as argument, which contains value for thread ID, prints the thread ID 
*/
void *printMsg(void *threadstart)
{     
   printf("Exiting Thread routine..........\n");
   pthread_exit(NULL);
}

/*
Function - execvp()
Description - The exec type system calls allow a process to run any program files, which include a binary executable or a shell script. 
execvp() system call requires two arguments:
 - The first argument is a character string that contains the name of a file to be executed.
 - The second argument is a pointer to an array of character strings. More precisely, its type is char **. When execvp() is executed, 
the program file given by the first argument will be loaded into the caller's address space and over-write the program there. 
Then, the second argument will be provided to the program and starts the execution. As a result, once the specified program file
starts its execution, the original program in the caller's address space is gone and is replaced by the new program.
execvp() returns a negative value if the execution fails
*/
int main(int argc, char *argv[])
{
	int status,t;
	pid_t pid;
	
	pthread_t thread;	
	printf("Before pthread_create\n");
	start_time_pthread = clock(); //to record start time before pthread_create()
	
	int err_num = pthread_create(&thread, NULL, printMsg, (void *)start_time_pthread); 
	end_time_pthread = clock(); //to record end time after pthread_create()
	if (err_num) 
	{  
		printf("ERROR; return code from pthread_create() is %d\n", err_num);
		exit(-1);
	}
	
    cpu_time_pthread = 1.0 * (end_time_pthread - start_time_pthread); //cpu time would be (end time - start time) for thread creation
    printf("CPU time for pthread: %f\n", (double)cpu_time_pthread);

	printf("Before fork\n");
	start_time = clock(); //to record start time before fork()
	if ((pid = fork()) == 0) // child
	{
		printf("Inside Child : PID[%d] , next calling execvp()\n", pid);
		char *args[]={"./problem3_new",NULL};
		execvp("./problem3_new",args);
	}
	else if (pid < 0) // failed to fork
	{
		printf("Failed to fork : %d\n", pid);
		exit(1);
	}
	else  // parent
	{
		printf("Inside Parent : PID[%d]\n", pid);
		end_time1 = clock(); //to record end time after fork() in parent
		cpu_time1 = 1.0 *(end_time1 - start_time);	
		printf("CPU time for fork inside parent: %f\n", (double)cpu_time1);
	}
	
	err_num = wait(&status); //wait for child to end
	end_time2 = clock();  //to record end time after fork() after child process exits
	if(err_num < 0)
		printf("wait failed, error code: %d\n", err_num);
	else
		printf("wait success, child : %d exited\n",err_num);
	
	cpu_time2 = 1.0 * (end_time2 - start_time);
	printf("CPU time for fork after child ends: %f\n", (double)cpu_time2);
	return 0;
}