/************************************************
GPS模块的驱动，在TX2440A开发板上做测试

维护记录：  2010-06-10  V1.0    

linux内核：2.6.31

硬件接法：
		CH0 --> GPH0
		CH1 --> GPH1
*************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/device.h>
#include <linux/gpio.h>

#define DEVICE_NAME	"TX2440-gps"    /* 设备名称 */		
static int GPS_Major = 0;			/* 主设备号 */

static int TX2440_gps_open(struct inode *inode, struct file *file)
{
	s3c2410_gpio_cfgpin(S3C2410_GPH(0), S3C2410_GPIO_OUTPUT);
	s3c2410_gpio_cfgpin(S3C2410_GPH(1), S3C2410_GPIO_OUTPUT);
	s3c2410_gpio_setpin(S3C2410_GPH(0), 1);
	s3c2410_gpio_setpin(S3C2410_GPH(1), 0);
	return 0;
}

static int TX2440_gps_release(struct inode *inode, struct file *file)
{
	s3c2410_gpio_setpin(S3C2410_GPH(0), 0);
	s3c2410_gpio_setpin(S3C2410_GPH(1), 0);
	return 0;
}

static int TX2440_gps_ioctl( struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{

}

static struct file_operations TX2440_gps_fops =
{
	.owner  =   THIS_MODULE,
	.open   =   TX2440_gps_open, 
	.release =  TX2440_gps_release,
	.ioctl  =   TX2440_gps_ioctl,
};

static struct class *gps_class;

static int __init TX2440_gps_init(void)
{

	printk("TX2440 GPS DRIVER MODULE INIT\n");

	GPS_Major = register_chrdev(0, DEVICE_NAME, &TX2440_gps_fops);
	if (GPS_Major < 0)
	{
		printk(DEVICE_NAME " can't register major number\n");
		return GPS_Major;
	}
	printk("register TX2440-GPS Driver OK! Major = %d\n", GPS_Major);

	gps_class = class_create(THIS_MODULE, DEVICE_NAME);
	if(IS_ERR(gps_class))
	{
		printk("Err: failed in TX2440-GPS class. \n");
		return -1;
	}

	device_create(gps_class, NULL, MKDEV(GPS_Major, 0), NULL, DEVICE_NAME);

	printk(DEVICE_NAME " initialized\n");
	return 0;
}

static void __exit TX2440_gps_exit(void)
{
	printk("TX2440 GPS DRIVER MODULE EXIT\n");
	unregister_chrdev(GPS_Major, DEVICE_NAME);
	device_destroy(gps_class, MKDEV(GPS_Major, 0));
	class_destroy(gps_class);
}

module_init(TX2440_gps_init);
module_exit(TX2440_gps_exit);

MODULE_AUTHOR("www.txmcu.com");	
MODULE_DESCRIPTION("TX2440 GPS Driver");	
MODULE_LICENSE("GPL");

