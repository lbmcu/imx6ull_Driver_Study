#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <asm/uaccess.h>
#include <asm/io.h>

static int major;
static struct class *led_class;

//register setting
/*
 * IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3: 
 * 	Address: 229_0000h base + 14h offset = 229_0014h
 */
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
/*
 * GPIO5_GDIR 地址：020A_C004h
*/
static volatile unsigned int *GPIO5_GDIR;
/*
 * GPIO5_DR 地址：020A_C000h
*/
static volatile unsigned int *GPIO5_DR;


static ssize_t led_write(struct file *filp, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	char val;
	int ret;
	/* copy_form_user: get data from app */
	ret = copy_from_user(&val, buf, 1);
	if(ret)
	{
		return -EFAULT;
	}
	
	/* to set gpio register: out 1/0 */
	if(val)
	{
		/* set gpio to let led on -> gpio:0 */
		*GPIO5_DR &= ~(1<<3);
	}
	else
	{
		/* set gpio to let led off ->gpio:1 */
		*GPIO5_DR |= (1<<3);
	}

	return 1;
}

static int led_open(struct inode *inode, struct file *filp)
{
	/* enable gpio5	(CCM) ==> default enable
	 * configure GPIO5_IO3 as gpio (IOMUX)
	 * configure GPIO5_IO3 as output (GPIIO)
	 */
	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf; //clear reg data
	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5; //set pin as gpio

	*GPIO5_GDIR |= (1<<3);	//set pin model as output
	
	return 0;
}

static struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.write		= led_write,
	.open		= led_open,
};

//入口函数
static int __init led_init(void)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	major = register_chrdev(0, "100ask_led", &led_fops);

	/* ioremap */
	/*
	 * IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3: 
	 * 	Address: 229_0000h base + 14h offset = 229_0014h
	 */
	IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x02290014, 4);
	
	/*
	 * GPIO5_GDIR 地址：020A_C004h
	*/
	GPIO5_GDIR = ioremap(0x020AC004, 4);
	
	/*
	 * GPIO5_DR 地址：020A_C000h
	*/
	GPIO5_DR = ioremap(0x020AC000, 4);
	

	led_class = class_create(THIS_MODULE, "myled");
	device_create(led_class, NULL, MKDEV(major, 0), NULL, "myled");/* /dev/myled */
	return 0;
}

static void __exit led_exit(void)
{
	iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
	iounmap(GPIO5_GDIR);
	iounmap(GPIO5_DR);
	
	device_destroy(led_class, MKDEV(major, 0));
	class_destroy(led_class);
	
	unregister_chrdev(major, "100ask_led");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");


