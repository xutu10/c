/*************************************
按键的驱动，在TX2440A上做测试

维护记录： 2009-10-28 V1.0  
           2009-03-08 V1.1  

linux内核：2.6.31

硬件接法：
		KEY1 --> GPF4
		KEY2 --> GPF5
		KEY3 --> GPF6
		KEY4 --> GPF7
*************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
//#include <asm/arch/regs-gpio.h>
#include <mach/regs-gpio.h>
//#include <asm/hardware.h>
#include <mach/hardware.h>
#include <linux/device.h>
#include <linux/poll.h>
#include <linux/gpio.h>

#define BUFFER_SIZE 256
#define DEVICE_NAME	"TX2440-button" /* 设备名称 */
static int BUTTON_MAJOR	= 0;		/* 主设备号 */

static volatile int key_values [] = {0, 0, 0, 0};
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);
static volatile int ev_press = 0;

struct button_irq_desc 
{
	int irq;
	int pin;
	int pin_setting;
	int number;
	char *name;	
};

/* 用来指定按键所用的外部中断引脚及中断触发方式, 名字 */
static struct button_irq_desc button_irqs [] =
{
	{IRQ_EINT4,	S3C2410_GPF(4),	S3C2410_GPF4_EINT4,	0, "KEY1"}, /* K1 */
	{IRQ_EINT5,	S3C2410_GPF(5),	S3C2410_GPF5_EINT5,	1, "KEY2"}, /* K2 */
	{IRQ_EINT6,	S3C2410_GPF(6),	S3C2410_GPF6_EINT6,	2, "KEY3"}, /* K3 */
	{IRQ_EINT7,	S3C2410_GPF(7),	S3C2410_GPF7_EINT7,	3, "KEY4"}, /* K4 */
};

static void open_irqs(void)
{
	int i;
	
	for (i = 0; i < sizeof(button_irqs)/sizeof(button_irqs[0]); i++)
	{
		enable_irq(button_irqs[i].irq);
	}
}

static void close_irqs(void)
{
	int i;
	
	for (i = 0; i < sizeof(button_irqs)/sizeof(button_irqs[0]); i++)
	{
		disable_irq(button_irqs[i].irq);
	}
}

static irqreturn_t buttons_interrupt(int irq, void *dev_id)
{
	struct button_irq_desc *button_irqs = (struct button_irq_desc *)dev_id;
	int key_press = s3c2410_gpio_getpin(button_irqs->pin);

	if (key_press == 0)
	{
		mdelay(20);
		key_press = s3c2410_gpio_getpin(button_irqs->pin);
		if (key_press == 0)
		{
			key_values[button_irqs->number] = (button_irqs->number + 1);
			ev_press = 1; 
			wake_up_interruptible(&button_waitq);
		}			
	}
	return IRQ_RETVAL(IRQ_HANDLED);
}

static void register_irqsrv(void)
{
	int i;
	int err;

	for (i = 0; i < sizeof(button_irqs)/sizeof(button_irqs[0]); i++)
	{
		// 注册中断处理函数
		s3c2410_gpio_cfgpin(button_irqs[i].pin,button_irqs[i].pin_setting);
		err = request_irq(button_irqs[i].irq, buttons_interrupt, NULL, button_irqs[i].name, (void *)&button_irqs[i]);
		if (err)
			break;
	}

	if (err)
	{
		// 释放已经注册的中断
		i--;
		for (; i >= 0; i--)
		{
			disable_irq(button_irqs[i].irq);
			free_irq(button_irqs[i].irq, (void *)&button_irqs[i]);
		}
		return -EBUSY;
	}	
}

static void unregister_irqsrv(void)
{
	int i;
	
	for (i = 0; i < sizeof(button_irqs)/sizeof(button_irqs[0]); i++)
	{
		free_irq(button_irqs[i].irq, (void *)&button_irqs[i]);
	}
}

static int TX2440_button_open(struct inode *inode, struct file *file)
{
	open_irqs();
	printk("TX2440-BUTTON Driver Open Called!\n");	
	return 0;
}

static int TX2440_button_close(struct inode *inode, struct file *file)
{
	close_irqs();

	printk("TX2440-BUTTON Driver Release Called!\n");	
	return 0;
}

static int TX2440_button_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	unsigned long err;

	if (!ev_press)
	{
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		else
			wait_event_interruptible(button_waitq, ev_press);
	}

	ev_press = 0;

	err = copy_to_user(buff, (const void *)key_values, min(sizeof(key_values), count));
	memset((void *)key_values, 0, sizeof(key_values));
	
	return err ? -EFAULT : min(sizeof(key_values), count);
}

static unsigned int TX2440_button_poll( struct file *file, struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &button_waitq, wait);
	if (ev_press)
		mask |= POLLIN | POLLRDNORM;
	return mask;
}

static struct file_operations TX2440_button_fops =
{
	.owner	=	THIS_MODULE,
	.open	=	TX2440_button_open,
	.release	=	TX2440_button_close, 
	.read	=	TX2440_button_read,
	.poll	=	TX2440_button_poll,
};

static struct class *button_class;

static int __init TX2440_button_init(void)
{
	printk("TX2440 BUTTON DRIVER MODULE INIT\n");
	
	register_irqsrv();
	close_irqs();

	BUTTON_MAJOR = register_chrdev(0, DEVICE_NAME, &TX2440_button_fops);
	if (BUTTON_MAJOR < 0)
	{
		printk(DEVICE_NAME " can't register major number\n");
		return BUTTON_MAJOR;
	}
	printk("register TX2440-Button Driver OK! Major = %d\n", BUTTON_MAJOR);

	button_class = class_create(THIS_MODULE, DEVICE_NAME);
	if(IS_ERR(button_class))
	{
		printk("Err: failed in TX2440-Button class. \n");
		return -1;
	}

	device_create(button_class, NULL, MKDEV(BUTTON_MAJOR, 0), NULL, DEVICE_NAME);

	printk(DEVICE_NAME " initialized\n");
	return 0;
}

static void __exit TX2440_button_exit(void)
{
	printk("TX2440 BUTTON DRIVER MODULE EXIT\n");
	
	close_irqs();
	unregister_irqsrv();
	
	unregister_chrdev(BUTTON_MAJOR, DEVICE_NAME);
	device_destroy(button_class, MKDEV(BUTTON_MAJOR, 0));
	class_destroy(button_class);					
}

module_init(TX2440_button_init);
module_exit(TX2440_button_exit);

MODULE_AUTHOR("www.txmcu.com");
MODULE_DESCRIPTION("TX2440 Button Driver");
MODULE_LICENSE("GPL");

