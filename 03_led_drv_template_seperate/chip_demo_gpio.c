#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include "led_opr.h"
#include "led_resource.h"

static struct led_resource* led_res;

static int board_demo_led_init(int which)	/* 初始化LED，which-哪一个LED */
{
	//printk("%s %s line %d, led %d\n", __FILE__, __FUNCTION__, __LINE__, which);
	if(!led_res)
	{
		led_res = get_led_resource();
	}

	printk("init gpio: group = %d, pin = %d\n", GROUP(led_res->pin), PIN(led_res->pin));
	switch(GROUP(led_res->pin))
	{
		case 0:
		{
			printk("init pin of group 0 ...\n");
			break;
		}

		case 1:
		{
			printk("init pin of group 1 ...\n");
			break;
		}

		case 2:
		{
			printk("init pin of group 2 ...\n");
			break;
		}

		case 3:
		{
			printk("init pin of group 3 ...\n");
			break;
		}
	}
	
	return 0;
}

static int board_demo_led_ctl(int which, char status)	/* 控制LED，which-哪一个LED, status:1-亮, 0-灭 */
{
	//printk("%s %s line %d, led %d, %s\n", __FILE__, __FUNCTION__, __LINE__, which, status ? "on" : "off");
	printk("set led %s：group = %d, pin = %d\n", status ? "on" : "off", GROUP(led_res->pin), PIN(led_res->pin));
	switch(GROUP(led_res->pin))
	{
		case 0:
		{
			printk("set pin of group 0 ...\n");
			break;
		}

		case 1:
		{
			printk("set pin of group 1 ...\n");
			break;
		}

		case 2:
		{
			printk("set pin of group 2 ...\n");
			break;
		}

		case 3:
		{
			printk("set pin of group 3 ...\n");
			break;
		}
	}
	return 0;
}

static struct led_oprations board_demo_led_opr = {
	.init = board_demo_led_init,
	.led_ctl = board_demo_led_ctl,
};


struct led_oprations* get_board_led_opr(void)
{
	return &board_demo_led_opr;
}


