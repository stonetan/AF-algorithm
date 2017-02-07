#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/completion.h>

#include "ls_lens_timer.h"

static struct timer_list timer;

int ModifyTimer(unsigned long expires)
{
	mod_timer(&timer, jiffies + msecs_to_jiffies(expires));
	//mod_timer(&timer, jiffies + expires);
	//printk("expires=%d\n", expires);

	return 0;
}

int TimerInit(void* func, unsigned long dat, unsigned long expires)
{
	init_timer(&timer);
	
	timer.data = dat;
	timer.expires = jiffies + msecs_to_jiffies(expires);
	timer.function = func;

	add_timer(&timer);

	return 0;
}

int TimerExit(void)
{
	del_timer(&timer);

	return 0;
}

DECLARE_COMPLETION(comp);

static void TimerStop(unsigned long dat)
{
	complete(&comp);
}

int MotorDelay(unsigned int ms)
{
	TimerInit(TimerStop, 0, ms);
	wait_for_completion(&comp);
	TimerExit();

	return 0;
}

