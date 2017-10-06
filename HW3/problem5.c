/******************************************************************************
  File: Vihanga Bare
  Description: Multi-threaded I/O source code 
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
#include <signal.h>
#include <stdint.h>

#define WRITESIZE 50
#define TOP 5

/*Shared struct */
struct Count
{
	uint32_t line_count;
	uint32_t word_count;
	uint32_t char_count;
};
struct Count *cnt;

/* global variables */
uint32_t occurences[200] = {0};
uint32_t at_position[200] = {0};	
uint32_t exit_flag_t1 = 0;
uint32_t exit_flag_t2 = 0;
uint32_t count;
char ch;
char **dict;
FILE *fp_write;
pthread_t t1,t2;
pthread_mutex_t mutex;  	/* mutex lock for buffer */
pthread_cond_t file_read;   /*  condition variable for read */
pthread_cond_t file_print;  /*  condition variable for print stats  */

/* signal handler function prototypes */
void sighandler_sigusr1(int signum);
void sighandler_sigusr2(int signum);
void sighandler_sigint(int signum);

/*
Function - void sighandler_sigusr1(int signum);
Description - Signal handler for SIGUSR1. It signals the condtion on which processing thread t1 is waiting
*/
void sighandler_sigusr1(int signum)
{
   printf("Caught signal %d, coming out...\n", signum);
   int32_t retval = pthread_cond_signal(&file_read); //It signals the condtion on which processing thread t1 is waiting
   	if(retval != 0)
	{
		printf("condition signal failed, error code - %d\n", retval);
	}
}

/*
Function - void sighandler_sigusr1(int signum);
Description - Signal handler for SIGUSR2. It signals the condtion on which printing thread t2 is waiting
*/
void sighandler_sigusr2(int signum)
{
	printf("Caught signal %d, coming out...\n", signum);
	int32_t retval = pthread_cond_signal(&file_print); //It signals the condtion on which printing thread t2 is waiting
	if(retval != 0)
	{
		printf("condition signal failed, error code - %d\n", retval);
	}
}
/*
Function - void sighandler_sigusr1(int signum);
Description - Signal handler for SIGINT or CTRL + C  It destroys mutexes, cond variables, closes file handle and exits all the threads gracefully
*/
void sighandler_sigint(int signum)
{
	printf("Caught signal %d, coming out...\n", signum);
	
	exit_flag_t1 = 1; //set exit flag for thread t1
	int32_t retval = pthread_cond_signal(&file_read);
	if(retval != 0)
	{
		printf("condition signal failed, error code - %d\n", retval);
	}
	retval = pthread_join(t1,NULL);
	if(retval != 0)
	{
		printf("pthread join failed, error code - %d\n", retval);
	}
	
	exit_flag_t2 = 1;  //set exit flag for thread t2
	retval = pthread_cond_signal(&file_print);
	if(retval != 0)
	{
		printf("condition signal failed, error code - %d\n", retval);
	}
	
	retval = pthread_join(t2,NULL);
	if(retval != 0)
	{
		printf("pthread join failed, error code - %d\n", retval);
	}
	
	//wait for t1, t2 to join and then exit gracefully. destroy mutexes, cond variables, closes file handle 
	
	fclose(fp_write);
	
	retval = pthread_mutex_destroy(&mutex);
	if(retval != 0)
	{
		printf("mutex destroy failed, error code - %d\n", retval);
	}	
	retval =  pthread_cond_destroy(&file_read);
	if(retval != 0)
	{
		printf("cond destroy failed, error code - %d\n", retval);
	}
	retval =  pthread_cond_destroy(&file_print);
	if(retval != 0)
	{
		printf("cond destroy failed, error code - %d\n", retval);
	}
	exit(0);
}

/*
Function - *fileStats(void *threadstart)
Description - Thread function for processing word, line and char count. It also calculates frequency of entered words 
to help us find top 5 wordsentered by user
*/

void *fileStats(void *threadstart)
{     
	printf("Inside file stats..........\n");
	
	long pos;
	char string[200];
	int ret;
	int flag;
	
	printf("file pointer position : %ld \n", pos);
	
	while(1)
	{	
		ret = pthread_mutex_lock(&mutex); //lock mutex
		if(ret != 0)
		{
			printf("condition signal failed, error code - %d\n", ret);
		}
		ret= pthread_cond_wait(&file_read, &mutex); //wait for condition from SIGUSR1, SIGINT
		if(ret != 0)
		{
			printf("condition signal failed, error code - %d\n", ret);
		}
		if(exit_flag_t1 == 1)
		{
			pthread_mutex_unlock(&mutex); //unlock mutex before exit
			break;
        }
		//initiliase all count variables to 0
		cnt->line_count = 0;
		cnt->word_count = 0;
		cnt->char_count = 0;
		count=0;
		memset(occurences,0,sizeof(occurences));
		//memset(dict,0,sizeof(dict));
		
		
		pos = ftell(fp_write); //record current position of file pointer
		printf("file pointer position after enter data : %ld \n", pos);
		ret = fseek(fp_write, 0, SEEK_SET); //return file pointer to beg of file
		if(ret < 0)
		{
			printf("fseek failed , error code - %d\n", ret);
		}
	
		printf("file pointer after seek_set : %ld \n", ftell(fp_write));
		if(fp_write)
		{
			//Repeat until current position of file pointer not reached 'pos'	
		   while(ftell(fp_write) != pos)
		   {
				ch=getc(fp_write);
				// Increment character count if NOT new line or space
				if (ch != '\n')	{ cnt->char_count++; }
				  
				// Increment word count if new line or space character
				if (ch == ' ' || ch == '\n') 	
				{ 
					cnt->word_count++; 
				}  
				// Increment line count if new line character
				if (ch == '\n') 				
				{ 
					cnt->line_count++; 
				}
		    }
			//return file pointer to beg of file
			ret = fseek(fp_write, 0, SEEK_SET);
			if(ret < 0)
			{
				printf("fseek failed , error code - %d\n", ret);
			}

			printf("file pointer after seek_set : %ld \n", ftell(fp_write));
			printf("position variable value : %ld \n", pos);
			
			//Repeat until current position of file pointer not reached 'pos'
			while(ftell(fp_write) != pos)
			{
				//printf(" fp position before fscanf: %ld\n", ftell(fp_write));
				fscanf(fp_write,"%s",string); 
				flag = 0;
				//check if string exists in dict[i], increment occurences[i] if yes, if not add it to dict
				for(uint32_t i = 0; i < count;i++)
				{
					if(strncasecmp(string, dict[i],strlen(string)) == 0) 
					{
						occurences[i]++;
						flag = 1;
						printf(" occurences of %s : %d \n",dict[i], occurences[i]);
						break;
					}
				}
				//flag set if match was found by strcasecmp
				if(flag != 1)
				{
					strncpy(dict[count],string,strlen(string));
					occurences[count]++;
					count++;	
				}
				//printf(" fp position after case match : %ld\n", ftell(fp_write));
			}
		   
		   for(uint32_t i = 0; i < count; i++)
		   {
			   printf(" dict word: %s\n", dict[i]);
			   printf(" word occurences %d\n", occurences[i]);
			   at_position[i] = i;
		   }
		   
		   //sort descending
		   uint32_t temp;
		   for(uint32_t i = 0; i < count; i++)
		   {
				for(uint32_t j = i; j < count; j++)
				{
					if(occurences[i] < occurences[j])
					{
						temp = occurences[i];
						occurences[i] = occurences[j];
						occurences[j] = temp;
						
						temp = at_position[i];
						at_position[i] = at_position[j];
						at_position[j] = temp;
					}
				}
		   }
		}
		else
		{
			printf("Failed to open the file\n");
		}
		ret = pthread_mutex_unlock(&mutex); //unlock mutex
		if(ret != 0)
		{
			printf("mutex unlock failed , error code - %d\n", ret);
		}
		
	}
	printf("\exit t1 \n");
	pthread_exit(NULL);
}
/*
Function - *statsPrint(void *threadstart)
Description - Thread function for printing word, line and char count. It also prints top 5 words entered by user with their frequency
*/

void *statsPrint(void *threadstart)
{     
	printf("inside file stats print..........\n");
	int ret,loop;
	while(1)
	{
		ret = pthread_mutex_lock(&mutex); //lock mutex
		if(ret != 0)
		{
			printf("mutex lock failed , error code - %d\n", ret);
		}
		ret = pthread_cond_wait(&file_print, &mutex); //wait for signal from sighandler_sigusr2, sigint
		if(ret != 0)
		{
			printf("pthread condition wait , error code - %d\n", ret);
		}
		if(exit_flag_t2 == 1)
		{
			ret = pthread_mutex_unlock(&mutex); //unlock mutex after signal from sigint
			if(ret != 0)
			{
				printf("pthread mutex unlock failed , error code - %d\n", ret);
			}
			break;
        }
		printf("Lines : %d \n", cnt->line_count);
		printf("Words : %d \n", cnt->word_count);
		printf("Characters : %d \n", cnt->char_count);
		
		if(count < TOP)
		{
			loop = count;
		}
		else
			loop = count;
			
		printf(" Top 5 words and occurences \n");	
		for(int i = 0; i < loop ; i++)
		{
		   int location = at_position[i];
		   printf(" Word %s\n", dict[location]);
		   printf(" Occurences %d\n", occurences[i]);
		}
		ret = pthread_mutex_unlock(&mutex); //unlock mutex
		if(ret != 0)
		{
			printf("pthread mutex unlock failed , error code - %d\n", ret);
		}
	}
	printf("\exit t2 \n");
	pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
	char writebuf[100];
	char filename[20];
	int ret;
	long pos = 0;
	
	cnt = (struct Count*)malloc(sizeof(struct Count)); //allocate memory for shared struct
	
	dict = (char **)malloc(200*sizeof(char *)); //allocate memory for dictionary of words
	for (int i = 0; i < 200; i++)
		dict[i] = (char*)calloc(20,sizeof(char));
	
	ret = pthread_mutex_init(&mutex, NULL);
	if(ret !=0)
	{
		printf("mutex initialisation failed, error code - %d\n", ret);
	}
	ret = pthread_cond_init(&file_read, NULL);
	if(ret !=0)
	{
		printf("condition var initialisation failed, error code - %d\n", ret);
	}
	ret = pthread_cond_init(&file_print, NULL);
	if(ret !=0)
	{
		printf("condition var initialisation failed, error code - %d\n", ret);
	}
	
	printf("Enter file name....\n");
	scanf("%s",filename); //wait for user to input file name
	
	fp_write = fopen(filename,"w+");
	
	//initialise signal handler
	signal(SIGUSR1, sighandler_sigusr1);
	signal(SIGUSR2, sighandler_sigusr2);
	signal(SIGINT, sighandler_sigint);
	
	
	//creating my processing thread
	
	int err_num = pthread_create(&t1, NULL, fileStats, (void *) t1); 
	if (err_num) 
	{  
		printf("ERROR; return code from pthread_create() is %d\n", err_num);
		exit(-1);
	}
	//creating my printing thread
	err_num = pthread_create(&t2, NULL, statsPrint, (void *) t2); 
	if (err_num) 
	{  
		printf("ERROR; return code from pthread_create() is %d\n", err_num);
		exit(-1);
	}	
	printf("Enter data....Press CTRL + C / Ctrl + D to exit\n");
	fgets(writebuf,WRITESIZE,stdin);
	while(fgets(writebuf,WRITESIZE,stdin) != NULL)
	{
		if(strncmp(writebuf,"end",3) == 0)
		{
			break;
		}
		ret = pthread_mutex_lock(&mutex); //lock mutex
		if(ret !=0)
		{
			printf("mutex lock failed, error code - %d\n", ret);
		}
		if(fwrite(writebuf,strlen(writebuf),1,fp_write)<0)
		{
		  printf("error writting file\n");
		  exit(1);
		}
		ret = pthread_mutex_unlock(&mutex); //unlock mutex
		if(ret !=0)
		{
			printf("mutex unlock failed, error code - %d\n", ret);
		}

	}
	
	printf("\exit main \n");	
	return 0;
}