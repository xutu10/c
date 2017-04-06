/************************************************
LED的驱动，在TX2440A开发板上做测试

维护记录：  2009-10-18  V1.0    

linux内核：2.6.31

硬件接法：
		LED1 --> GPF0
		LED2 --> GPF1
		LED3 --> GPF2
		LED4 --> GPF3
		低电平点亮
驱动用法：
		设备名称：TX2440-led
		点亮一个灯：LED_ON
		熄灭一个灯：LED_OFF
		点亮所有灯：ALL_LED_ON
		熄灭所有灯：ALL_LED_OFF
说明：
		内核源码里已经有LED的驱动，将它们注释掉
		在arch/arm/plat-s3c24xx/common-smdk.c中
		将GPF0-GPF3设为输出，全部输出低电平
*************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/irq.h>
//#include <asm/arch/regs-gpio.h>
#include <mach/regs-gpio.h>
//#include <asm/hardware.h>
#include <mach/hardware.h>
#include <linux/device.h>
#include <linux/gpio.h>

#define DEVICE_NAME	"TX2440-led"    /* 设备名称 */		
static int LED_Major = 0;			/* 主设备号 */

#define LED_OFF	         0
#define LED_ON	         1
#define ALL_LED_OFF      3
#define ALL_LED_ON       4

/* 用来指定LED所用的GPIO引脚 */
static unsigned long led_table [] =
{
	S3C2410_GPF(0),
	S3C2410_GPF(1),
	S3C2410_GPF(2),
	S3C2410_GPF(3),
};

static int TX2440_led_open(struct inode *inode, struct file *file)
{
//	MOD_INC_USE_COUNT;
	printk("TX2440-LED Driver Open Called!\n");
	return 0;
}

static int TX2440_led_release(struct inode *inode, struct file *file)
{
//	MOD_DEC_USE_COUNT;
	printk("TX2440-LED Driver Release Called!\n");
	return 0;
}

static int TX2440_led_ioctl( struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	int i;
	if (arg > 4)
	{
		return -EINVAL;
	}
	switch(cmd)
	{
		case LED_ON:  //set the pin
			s3c2410_gpio_setpin(led_table[arg], 0);
			break;

		case LED_OFF:  //clr the pin
			s3c2410_gpio_setpin(led_table[arg], 1);
			break;
			
		case ALL_LED_ON:  //set all pin
			for (i = 0; i < 4; i++)
				s3c2410_gpio_setpin(led_table[i], 0);
			break;
			
		case ALL_LED_OFF:  //clr all pin
			for (i = 0; i < 4; i++)
				s3c2410_gpio_setpin(led_table[i], 1);
			break;

		default:
			return -EINVAL;
	}
}

static struct file_operations TX2440_led_fops =
{
	.owner  =   THIS_MODULE,
	.open   =   TX2440_led_open, 
	.release =  TX2440_led_release,
	.ioctl  =   TX2440_led_ioctl,
};

static struct class *led_class;

static int __init TX2440_led_init(void)
{

	printk("TX2440 LED DRIVER MODULE INIT\n");

	LED_Major = register_chrdev(0, DEVICE_NAME, &TX2440_led_fops);
	if (LED_Major < 0)
	{
		printk(DEVICE_NAME " can't register major number\n");
		return LED_Major;
	}
	printk("register TX2440-LED Driver OK! Major = %d\n", LED_Major);

	led_class = class_create(THIS_MODULE, DEVICE_NAME);
	if(IS_ERR(led_class))
	{
		printk("Err: failed in TX2440-LED class. \n");
		return -1;
	}

	device_create(led_class, NULL, MKDEV(LED_Major, 0), NULL, DEVICE_NAME);

	printk(DEVICE_NAME " initialized\n");
	return 0;
}

static void __exit TX2440_led_exit(void)
{
	printk("TX2440 LED DRIVER MODULE EXIT\n");
	unregister_chrdev(LED_Major, DEVICE_NAME);
	device_destroy(led_class, MKDEV(LED_Major, 0));
	class_destroy(led_class);
}

module_init(TX2440_led_init);
module_exit(TX2440_led_exit);

MODULE_AUTHOR("www.txmcu.com");		
MODULE_DESCRIPTION("TX2440 LED Driver");	
MODULE_LICENSE("GPL");

