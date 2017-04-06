/************************************************
ADC的驱动，在TX2440A开发板上做测试

维护记录：  2009-10-20  V1.0    

linux内核：2.6.31

硬件接法：
		RP1 --> ADC CH0
		RP2 --> ADC CH1
驱动用法：
		设备名称：TX2440-adc
*************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <asm/arch/regs-gpio.h>
#include <mach/regs-gpio.h>
#include <plat/regs-adc.h>
//#include <asm/hardware.h>
#include <mach/hardware.h>
#include <linux/device.h>

#define DEVICE_NAME	"TX2440-adc"    /* 设备名称 */		
static int ADC_Major = 0;			/* 主设备号 */

static void __iomem *base_addr;
static int adc_ch;

#define ADCCON      (*(volatile unsigned long *)(base_addr + S3C2410_ADCCON))	//ADC control
#define ADCTSC      (*(volatile unsigned long *)(base_addr + S3C2410_ADCTSC))	//ADC touch screen control
#define ADCDLY      (*(volatile unsigned long *)(base_addr + S3C2410_ADCDLY))	//ADC start or Interval Delay
#define ADCDAT0     (*(volatile unsigned long *)(base_addr + S3C2410_ADCDAT0))	//ADC conversion data 0
#define ADCDAT1     (*(volatile unsigned long *)(base_addr + S3C2410_ADCDAT1))	//ADC conversion data 1

void ADC_Init(void)
{
    ADCDLY = 100;				//ADC转换延时	
    ADCCON = (0 << 0)     |	// ADC转换设置 未设置
              (0 << 1)     |    // 读AD数据触发AD转换 未使用
              (0 << 2)     |    // StandBy模式选择 为普通操作模式
//              (ch << 3)    |    // ADC通道选择 ch
              (49 << 6)    | 	// CLKDIV = Fpclk /49+1/5 ，即转换时钟为1MHz  Fpclk = 10M   ADC转换频率400K
              (1 << 14)   ;	    // 使能软件预设值
}

int AD_READ(void) 
{
    int val;
 	val = 0;
 	ADCCON |= 0x1;         		 //ADC转换通过读操作来启动
 	
	while(ADCCON&0x1);		 //判断ADC转换是否结束
	while(!ADCCON&0x8000);		 //判断ADC转换是否结束
       
	val = (ADCDAT0 &0x03ff);    //取出ADC转换值
	
	mdelay(100);
	return val;
}

static int TX2440_adc_open(struct inode *inode, struct file *file)
{
	printk("TX2440-ADC Driver Open Called!\n");
	return 0;
}

static int TX2440_adc_release(struct inode *inode, struct file *file)
{
	printk("TX2440-ADC Driver Release Called!\n");
	return 0;
}

static int TX2440_adc_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int adc_val = AD_READ();
	copy_to_user(buf, &adc_val, sizeof(adc_val));
	
	return sizeof(adc_val);
}

static int TX2440_adc_ioctl( struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
		adc_ch = cmd;
		ADCCON &= 0xFFC7;
		ADCCON |= (adc_ch << 3);

		return 0;
}


static struct file_operations TX2440_adc_fops =
{
	.owner  =   THIS_MODULE,
	.open   =   TX2440_adc_open, 
	.release =  TX2440_adc_release,
	.ioctl  =   TX2440_adc_ioctl,
	.read   =   TX2440_adc_read,
};

static struct class *adc_class;

static int __init TX2440_adc_init(void)
{

	printk("TX2440 ADC DRIVER MODULE INIT\n");
	
	base_addr=ioremap(S3C2410_PA_ADC,0x20);
	if (base_addr == NULL) {
		printk(KERN_ERR "Failed to remap register block\n");
		return -ENOMEM;
	}

	ADC_Init();  //ADC初始化
		
	ADC_Major = register_chrdev(0, DEVICE_NAME, &TX2440_adc_fops);
	if (ADC_Major < 0)
	{
		printk(DEVICE_NAME " can't register major number\n");
		return ADC_Major;
	}
	printk("register TX2440-ADC Driver OK! Major = %d\n", ADC_Major);

	adc_class = class_create(THIS_MODULE, DEVICE_NAME);
	if(IS_ERR(adc_class))
	{
		printk("Err: failed in TX2440-ADC class. \n");
		return -1;
	}

	device_create(adc_class, NULL, MKDEV(ADC_Major, 0), NULL, DEVICE_NAME);

	printk(DEVICE_NAME " initialized\n");
	return 0;
}

static void __exit TX2440_adc_exit(void)
{
	printk("TX2440 ADC DRIVER MODULE EXIT\n");
	unregister_chrdev(ADC_Major, DEVICE_NAME);
	device_destroy(adc_class, MKDEV(ADC_Major, 0));
	class_destroy(adc_class);
}

module_init(TX2440_adc_init);
module_exit(TX2440_adc_exit);

MODULE_AUTHOR("www.txmcu.com");		
MODULE_DESCRIPTION("TX2440 ADC Driver");	
MODULE_LICENSE("GPL");
