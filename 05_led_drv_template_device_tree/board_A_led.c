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
#include <linux/platform_device.h>

#include "led_resource.h"

static void led_dev_release(struct device *dev)
{
}

static struct resource board_A_resource[] = {
	{
		.start	= GROUP_PIN(5,3),
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= GROUP_PIN(5,4),
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= GROUP_PIN(5,5),
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= GROUP_PIN(5,6),
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= GROUP_PIN(5,7),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device board_A_led_dev = {
	.name		= "100ask_led",
	.id			= 0,
	.resource	= board_A_resource,
	.num_resources	= ARRAY_SIZE(board_A_resource),
	.dev = {
            .release = led_dev_release,
     },
};

static int __init led_dev_init(void)
{
	int err;
	
	err = platform_device_register(&board_A_led_dev);
	
	return 0;
}

static void __exit led_dev_exit(void)
{
	platform_device_unregister(&board_A_led_dev);
}

module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");