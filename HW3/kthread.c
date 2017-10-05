/*
File name - kthread_module.c
description - kthread create cpu time profiler code
*/

#include<linux/sched.h> //for wake process
#include<linux/kthread.h> //kthread_create() //kthread_run()
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/timer.h>
#include<linux/jiffies.h>
#include<linux/ktime.h>
#include<linux/time.h>

MODULE_LICENSE("GPL");

static struct task_struct *test_task;
long start_time;
long total_time;
int i;
struct timeval t0, t1;
long temp;

int test_thread(void *dummy)
{
	printk(KERN_WARNING"Inside test thread.....\n");
	return 0;
}

int test_init_module(void)
{
	start_time = jiffies; //start time for jiffies
	printk(KERN_WARNING"Jiffies %ld \n",jiffies);
	
	
	do_gettimeofday(&t0); //start time for get time of day
	test_task = kthread_create(test_thread,NULL,"test_task");	
	if(!test_task)
	{
		printk(KERN_WARNING"Error in kthread creation \n");
	}
	total_time = jiffies - start_time;
	do_gettimeofday(&t1); //end time for get time of day
	temp = (t1.tv_usec - t0.tv_usec);
	printk(KERN_WARNING"temp variable value : %ld \n",temp);
	printk(KERN_WARNING"Jiffies difference total time - start time : %ld \n",total_time);
	//wake_up_process(test_task);
	return 0;
}

void test_cleanup_module(void)
{
	if(test_task)
	{
		kthread_stop(test_task);
		printk(KERN_WARNING"Exiting kernel module\n");
		//test_task = NULL;
	}
}

module_init(test_init_module);
module_exit(test_cleanup_module);
