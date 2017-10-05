/*
Filename - traverse_process.c
Description - travers parents of current processes, and list all current processes
*/
 
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/sched/signal.h>

MODULE_LICENSE("GPL");

int test_init_module(void)
{
	struct task_struct *task;
	struct task_struct *task_parent;
	struct task_struct *task_children;
	struct list_head *list;
	int counter = 0;
	task = current; //macro for current running process
	for_each_process(task)
	{
		printk(KERN_WARNING"Name of process :%s PID : [%d] Process state : %ld \n",task->comm, task->pid, task->state);
	}
	printk(KERN_WARNING"\n");
	task = current;
	do
	{
		task_parent = task->parent;
		
		printk(KERN_WARNING"Name of process :%s PID : [%d] Process state : %ld \n",task->comm, task->pid, task->state);
		printk(KERN_WARNING" Process Parent : %s Parent PID :[%d] \n",task_parent->comm, task_parent->pid);
		list_for_each(list,&task->children)
		{
			task_children = list_entry(list, struct task_struct,sibling);
			counter++;		
		}
		printk(KERN_WARNING" Number of children : [%d] \n",counter);
		task = task_parent;
	}while(task_parent->pid != 0);
	return 0;
}

void test_cleanup_module(void)
{
	printk(KERN_WARNING"exiting traverse process module........\n");
}
module_init(test_init_module);
module_exit(test_cleanup_module);
