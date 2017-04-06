/*************************************
PWM的驱动，在TX2440A上做测试

维护记录： 2009-10-28 V1.0  

linux内核：2.6.31		
*************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <asm/irq.h>
#include <plat/regs-timer.h>
#include <mach/regs-gpio.h>
#include <mach/map.h>
#include <mach/regs-irq.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <asm/uaccess.h>
#include <asm/system.h>

#define DEVICE_NAME	"TX2440-Beep"
static int BEEP_MAJOR=0;		/* 主设备号 */

static int TX2440_beep_ioctl(
	struct inode *inode, 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	unsigned long temp;
	if(cmd <= 0)
	{
		temp = __raw_readl(S3C2410_GPBCON);	//GPBCON
		temp &= ~3;
		temp |= 1;
		__raw_writel(temp, S3C2410_GPBCON);

		temp = __raw_readl(S3C2410_GPBDAT);	//GPBDAT
		temp &= ~1;
		__raw_writel(temp, S3C2410_GPBDAT);
	}
	else
	{
		temp = __raw_readl(S3C2410_GPBCON);	//GPBCON
		temp &= ~3;
		temp |= 2;
		__raw_writel(temp, S3C2410_GPBCON);

		temp = __raw_readl(S3C2410_TCFG0);	//TCFG0
		temp &= ~0xff;
		temp |= 15;
		__raw_writel(temp, S3C2410_TCFG0);

		temp = __raw_readl(S3C2410_TCFG1);	//TCFG1
		temp &= ~0xf;
		temp |= 2;
		__raw_writel(temp, S3C2410_TCFG1);

		temp = (50000000/128)/cmd;
		__raw_writel(temp, S3C2410_TCNTB(0));

		temp >>= 1;
		__raw_writel(temp, S3C2410_TCMPB(0));

		temp = __raw_readl(S3C2410_TCON);	//TCON
		temp &= ~0x1f;
		temp |= 0xb;
		__raw_writel(temp, S3C2410_TCON);

		temp &= ~2;
		__raw_writel(temp, S3C2410_TCON);	
	}
	return 0;
}

static struct file_operations TX2440_beep_fops = {
	.owner	=	THIS_MODULE,
	.ioctl	=	TX2440_beep_ioctl,
};

static struct class *beep_class;

static int __init TX2440_beep_init(void)
{
	printk("TX2440 BEEP DRIVER MODULE INIT\n");

	BEEP_MAJOR = register_chrdev(0, DEVICE_NAME, &TX2440_beep_fops);
	if (BEEP_MAJOR < 0) {
	  printk(DEVICE_NAME " can't register major number\n");
	  return BEEP_MAJOR;
	}
	printk("register TX2440-Beep Driver OK! Major = %d\n", BEEP_MAJOR);

	beep_class = class_create(THIS_MODULE, DEVICE_NAME);
	if(IS_ERR(beep_class))
	{
		printk("Err: failed in TX2440-Beep class. \n");
		return -1;
	}

	device_create(beep_class, NULL, MKDEV(BEEP_MAJOR, 0), NULL, DEVICE_NAME);

	printk(DEVICE_NAME " initialized\n");
	return 0;

}

static void __exit TX2440_beep_exit(void)
{
	unregister_chrdev(BEEP_MAJOR, DEVICE_NAME);
	device_destroy(beep_class, MKDEV(BEEP_MAJOR, 0));
	class_destroy(beep_class);						
}

module_init(TX2440_beep_init);
module_exit(TX2440_beep_exit);


MODULE_AUTHOR("www.txmcu.com");		
MODULE_DESCRIPTION("TX2440 Beep Driver");	
MODULE_LICENSE("GPL");		
