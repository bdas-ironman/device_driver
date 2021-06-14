/*
 * Copyright: Copyright (c) 2021 Bdas
 * All rights reserved.
 **/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/timer.h>


/* Timer Variable */

/*
 * (N.B. 1000usec == 1msec (mili sec) but unix will round up to HZ (.01sec)
 * The timer values are in 0.01 second (or 10ms) unit.
 * e.g:
 *  #define TIMEOUT (1000 * 5)    <<< 5000usec means 50 milli seconds
 *  #define TIMEOUT (1000 * 500)  <<< 500000usec means 500 milli seconds i.e .5sec
 **/

/*
 * Here it is going to use API msecs_to_jiffies(mili_sec).
 * We need 5sec of delay, that means we have to pass 5000 as parameter
 * to msecs_to_jiffies(5000) where 5000 will be considered as milisec.
 * 
 */

#define ONE_MSEC 1000
#define TIMEOUT (ONE_MSEC * 5)    /* 5000mili sec = 5sec seconds */

struct timer_list my_timer;
static unsigned int count = 0;

/*
 * TIMER FUNCTION : my_timer_fn()
 **/

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0))
static void my_timer_fn(struct timer_list *data)
#else
static void my_timer_fn(unsigned long data)
#endif
{
	int cpu;

	/* WRITE YOUR BUSINESS LOGIC */

	/* do your timer stuff here */
	cpu = smp_processor_id();
	printk("<%lu> Timer Callback function Called on CPU [%d], count = [%d]\n",
			jiffies, cpu, count++);

	/*
	 * Dump stack to verify, timer handler call path.
	 * Enable below line to see call stack of timer handler.
	 */

	// dump_stack();  //<<<<<<< Un-comment it and verify dmesg

	/*
	 * modify the timer for next time
	 * Re-enable timer. Because this function will be called only
	 * first time.
	 * If we re-enable this will work like periodic timer. 
	 **/

	mod_timer(&my_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}

/*
 * INIT MODULE
 **/

/*
 * please find timer list structure details of following kernel version
 * root]#uname -r
 * 3.10.0-1127.el7.x86_64
 *
 * crash> struct timer_list
 * struct timer_list {
 *    struct list_head entry;
 *    unsigned long expires; <<<<<<<<<<<<<<<<<<<
 *    struct tvec_base *base;
 *    void (*function)(unsigned long); <<<<<<<<<
 *    unsigned long data; <<<<<<<<<<<<<<<<<<<<<<
 *    int slack;
 *    int start_pid;
 *    void *start_site;
 *    char start_comm[16];
 *}
 */

int my_module_init(void)
{
	printk("Hello My Timer\n");

	/*  -- initialize the timer */
	init_timer(&my_timer);

	my_timer.function = my_timer_fn;
	my_timer.expires = jiffies + msecs_to_jiffies(TIMEOUT) ;
	my_timer.data = 0; /* data parameter is kernel version specific */

	/* -- TIMER START */
	add_timer(&my_timer);
	printk("%s(): module Insert...Done!!!\n", __func__);
	return 0;
}

/*
 * CLEANUP MODULE
 **/
void my_module_exit(void)
{
	del_timer(&my_timer);
	printk("%s(): module Remove...Done!!!\n", __func__);
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bdas");
MODULE_DESCRIPTION("A simple device driver - Kernel Timer");
MODULE_VERSION("1.0");
