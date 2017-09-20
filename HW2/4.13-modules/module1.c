#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/timer.h>

MODULE_LICENSE("GPL");

struct timer_list my_timer;

void my_timer_callback(unsigned long data)
{
        static int timer_count = 0;
	int ret_val;
	
	printk("Restarting timer........%d \n",jiffies_to_msecs(jiffies));

        ret_val = mod_timer(&my_timer,jiffies + msecs_to_jiffies(500));
        if(ret_val)
        {
                printk("mod_timer returns 1");
        }
	timer_count++;
	
	printk(" Timer count....%d \n",timer_count);
	
}

int init_timer_module(void)
{
        int ret_val;
        
        printk(KERN_ALERT "Installing Timer Module .........\n");
        setup_timer(&my_timer,my_timer_callback,0);
        
	printk("Starting the timer to fire in 500 ms %d \n",jiffies_to_msecs(jiffies));

        ret_val = mod_timer(&my_timer,jiffies + msecs_to_jiffies(500));

        if(ret_val)
        {
                printk(KERN_ALERT "mod_timer returns 1");
        }

        return 0;
}

void cleanup_timer_module(void)
{
        int ret_val;
	
	printk(KERN_ALERT "Removing Kernel Module ........");
        
        ret_val = del_timer(&my_timer);
        if(ret_val)
        {	
		printk(KERN_ALERT "del_timer returns 1 \n");
	}
	
	printk(KERN_ALERT "Timer Module uninstalling \n");
}

module_init(init_timer_module);
module_exit(cleanup_timer_module);

