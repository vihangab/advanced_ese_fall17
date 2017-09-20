#include<linux/kernel.h>
#include<linux/linkage.h>
#include<asm/uaccess.h>
#include<linux/syscalls.h>
#include<linux/slab.h>
#include<uapi/asm-generic/errno-base.h>

//asmlinkage long sys_mysyscall(int *buf, int len)
SYSCALL_DEFINE3(mysyscall, int32_t *, in_buf, int32_t *, sort_buf, int, len)
{
	//no. after SYSCALL_DEFINE(num) decides the number of arguments the syscall is taking as input
	
	int32_t *kbuffer;
	int32_t i,j,temp,count;

	printk(KERN_WARNING "Entering syscal....\n");

	kbuffer = (int32_t *)kmalloc(len * sizeof(int32_t),GFP_KERNEL); //allocate kernel buffer
	if(!kbuffer)
	{
		printk(KERN_DEBUG "Kernel Memory Allocation Failed.....\n"); //return error code if kernel memory allocation fails
		return EFAULT;
	}
	if(len <= 0)
	{
		printk(KERN_DEBUG "Invalid length of buffer.......\n"); //return error code for invalid length
		return EINVAL;
	}
	
	count = copy_from_user(kbuffer,in_buf,len * sizeof(int32_t)); //copy from user copies from user space buffer in_buf to kernel space buffer kbuffer
	if(count > 0)
	{
		printk(KERN_DEBUG "No. of bytes not copied from user space......%d", count);//copy from user returns the no. of bytes which were not copied from user space to kernel space
		return ENOMEM; //return error code if copy form user fails
	}
	
	printk(KERN_ALERT "Length of Buffer...%ld \n",(len-1) * sizeof(int32_t));
	printk(KERN_ALERT "Length of Buffer..%d \n",(len-1));
	printk(KERN_ALERT "copy_from_user done, started sorting buffer inside syscall.....\n");
	
	//simple sort 
	for(i=0;i<(len-1);i++)
	{
		for(j=i+1;j < len;j++)
		{	
			if(*(kbuffer+i) < *(kbuffer+j))
			{
				temp = *(kbuffer+i);
				*(kbuffer+i) = *(kbuffer+j);
				*(kbuffer+j) = temp;
			}
						
		}
		
	}
	printk(KERN_WARNING "Sort Completed\n");

	count = copy_to_user(sort_buf,kbuffer,len * sizeof(int)); //copy to user copies from kernel space to user space
	if(count < 0)
	{
		printk("No. of bytes not copied to user space......%d \n", count); //copy fron user returns the number of bytes which were not copied from kernel space to user space, use this to check error condition
		return ENOMEM; //return error code
	}
	printk(KERN_WARNING "copy_to_user done......\n");
	
	kfree(kbuffer); //free kernel memory
	printk(KERN_WARNING "Kernel memory freed...\n");
	
	// 0 = success status
	printk(KERN_WARNING "Exiting mysyscall.......\n");
	return 0;
}

