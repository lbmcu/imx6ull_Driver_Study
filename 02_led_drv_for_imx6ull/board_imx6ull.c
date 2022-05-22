#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <asm/io.h>

#include "led_opr.h"


static volatile unsigned int *CCM_CCGR1;
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
static volatile unsigned int *GPIO5_GDIR;
static volatile unsigned int *GPIO5_DR;


static int board_demo_led_init(int which)	/* 初始化LED，which-哪一个LED */
{
	unsigned int val;
	/*
	 * ioremap 地址映射
	 * 使能GPIO CCM 时钟控制模块 CCGR1
	 * 使能 GPIO5_3 引脚为GPIO功能 IOMUX
	 * 设置 GPIO5_3 引脚为输出模式
	 */

	if(which == 0)
	{
		//ioremap 地址映射
		if(!CCM_CCGR1)
		{
			//CCM_CCGR1 Address: 20C_4000h base + 6Ch offset = 20C_406Ch
			CCM_CCGR1 = ioremap(0x020C406C, 4);
			
			// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 Address: 229_0000h base + 14h offset = 229_0014h
			IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x02290014, 4);

			// GPIO5_GDIR Address: 020A_C000 base + 4h offset = 020A_C004h
			GPIO5_GDIR = ioremap(0x020AC004 ,4);

			// GPIO5_DR Address: 020A_C000 base + 0h offset = 020A_C000h
			GPIO5_DR = ioremap(0x020AC000, 4);
		}

		/* a. 使能 GPIO5
	 	* set CCM to enable GPIO5
	 	* CCM_CCGR1[CG15] 0x20C406C
	 	* bit[31:30] = 0b11
	 	*/
		*CCM_CCGR1 |= (3<<30);

		/* b. 设置 GPIO5_IO03 用于 GPIO
	 	* set IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3
	 	* to configure GPIO5_IO03 as GPIO
	 	* IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 0x2290014
	 	* bit[3:0] = 0b0101 alt5
	 	*/
		val = *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
		val &= ~(0xf);	//只有一次寄存器读写，提高效率
		val |= (0x5);
		*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = val;	//set ALT5:gpio

		/* b. 设置 GPIO5_IO03 作为 output 引脚
		* set GPIO5_GDIR to configure GPIO5_IO03 as output
		* GPIO5_GDIR 0x020AC000 + 0x4
		* bit[3] = 0b1
		*/
		*GPIO5_GDIR |= (1<<3);
	}
	
	return 0;
}

static int board_demo_led_ctl(int which, char status)	/* 控制LED，which-哪一个LED, status:1-亮, 0-灭 */
{
	//printk("%s %s line %d, led %d, %s\n", __FILE__, __FUNCTION__, __LINE__, which, status ? "on" : "off");

	if(which == 0)
	{
		if(status)
		{
			*GPIO5_DR &= ~(1<<3); //pin:0 on
		}
		else
		{
			*GPIO5_DR |= (1<<3);	//pin:1 off
		}
	}
	return 0;
}

static struct led_oprations board_demo_led_opr = {
	.num  = 1,
	.init = board_demo_led_init,
	.ctl  = board_demo_led_ctl,
};


struct led_oprations* get_board_led_opr(void)
{
	return &board_demo_led_opr;
}


