#include <linux/gfp.h>
#include "led_opr.h"	

static int board_demo_led_init(int which)	/* 初始化LED，which-哪一个LED */
{
	printk("%s %s line %d, led %d\n", __FILE__, __FUNCTION__, __LINE__, which);
	return 0;
}

static int board_demo_led_ctl(int which, char status)	/* 控制LED，which-哪一个LED, status:1-亮, 0-灭 */
{
	printk("%s %s line %d, led %d, %s\n", __FILE__, __FUNCTION__, __LINE__, which, status ? "on" : "off");
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


