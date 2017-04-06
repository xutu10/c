/************************************************
ADC����������TX2440A��������������

ά����¼��  2009-10-20  V1.0    

linux�ںˣ�2.6.31

Ӳ���ӷ���
		RP1 --> ADC CH0
		RP2 --> ADC CH1
�����÷���
		�豸���ƣ�TX2440-adc
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

#define DEVICE_NAME	"TX2440-adc"    /* �豸���� */		
static int ADC_Major = 0;			/* ���豸�� */

static void __iomem *base_addr;
static int adc_ch;

#define ADCCON      (*(volatile unsigned long *)(base_addr + S3C2410_ADCCON))	//ADC control
#define ADCTSC      (*(volatile unsigned long *)(base_addr + S3C2410_ADCTSC))	//ADC touch screen control
#define ADCDLY      (*(volatile unsigned long *)(base_addr + S3C2410_ADCDLY))	//ADC start or Interval Delay
#define ADCDAT0     (*(volatile unsigned long *)(base_addr + S3C2410_ADCDAT0))	//ADC conversion data 0
#define ADCDAT1     (*(volatile unsigned long *)(base_addr + S3C2410_ADCDAT1))	//ADC conversion data 1

void ADC_Init(void)
{
    ADCDLY = 100;				//ADCת����ʱ	
    ADCCON = (0 << 0)     |	// ADCת������ δ����
              (0 << 1)     |    // ��AD���ݴ���ADת�� δʹ��
              (0 << 2)     |    // StandByģʽѡ�� Ϊ��ͨ����ģʽ
//              (ch << 3)    |    // ADCͨ��ѡ�� ch
              (49 << 6)    | 	// CLKDIV = Fpclk /49+1/5 ����ת��ʱ��Ϊ1MHz  Fpclk = 10M   ADCת��Ƶ��400K
              (1 << 14)   ;	    // ʹ�����Ԥ��ֵ
}

int AD_READ(void) 
{
    int val;
 	val = 0;
 	ADCCON |= 0x1;         		 //ADCת��ͨ��������������
 	
	while(ADCCON&0x1);		 //�ж�ADCת���Ƿ����
	while(!ADCCON&0x8000);		 //�ж�ADCת���Ƿ����
       
	val = (ADCDAT0 &0x03ff);    //ȡ��ADCת��ֵ
	
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

	ADC_Init();  //ADC��ʼ��
		
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
