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

#define DEVICE_NAME	"mydriver"	
static int MYDRIVER_Major = 0;

static int mydriver_open(struct inode *inode, struct file *file)
{
	printk("My Driver Open Called!\n");
	return 0;
}

static int mydriver_release(struct inode *inode, struct file *file)
{
	printk("My Driver Release Called!\n");
	return 0;
}

static int mydriver_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	printk("My Driver Read Called!\n");
	return 0;
}

static int mydriver_write(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	printk("My Driver Write Called!\n");
	return 0;
}

static int mydriver_ioctl( struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{

}

static struct file_operations mydriver_fops =
{
	.owner  =   THIS_MODULE,
	.open   =   mydriver_open, 
	.release =  mydriver_release,
	.read   =   mydriver_read,
	.write  =   mydriver_write,
	.ioctl  =   mydriver_ioctl,
};

static struct class *mydriver_class;

static int __init mydriver_init(void)
{

	printk("MY DRIVER MODULE INIT\n");

	MYDRIVER_Major = register_chrdev(0, DEVICE_NAME, &mydriver_fops);
	if (MYDRIVER_Major < 0)
	{
		printk(DEVICE_NAME " can't register major number\n");
		return MYDRIVER_Major;
	}
	printk("register My Driver OK! Major = %d\n", MYDRIVER_Major);

	//注册一个类，使mdev可以在"/dev/"目录下面建立设备节点
	mydriver_class = class_create(THIS_MODULE, DEVICE_NAME);
	if(IS_ERR(mydriver_class))
	{
		printk("Err: failed in My Driver class. \n");
		return -1;
	}
	//创建一个设备节点，节点名为DEVICE_NAME
	device_create(mydriver_class, NULL, MKDEV(MYDRIVER_Major, 0), NULL, DEVICE_NAME);

	printk(DEVICE_NAME " initialized\n");
	return 0;
}

static void __exit mydriver_exit(void)
{
	printk("MY DRIVER MODULE EXIT\n");
	unregister_chrdev(MYDRIVER_Major, DEVICE_NAME);
	device_destroy(mydriver_class, MKDEV(MYDRIVER_Major, 0));
	class_destroy(mydriver_class);
}

module_init(mydriver_init);
module_exit(mydriver_exit);

MODULE_AUTHOR("www.txmcu.com");		
MODULE_DESCRIPTION("My Driver");	
MODULE_LICENSE("GPL");

