/************************************************
DS18B20的驱动，在TX2440A开发板上做测试

维护记录：  2009-10-30  V1.0   

linux内核：2.6.31

硬件接法：
		DQ --> GPB5
驱动用法：
		设备名称：TX2440-ds18b20
*************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
//#include <asm/arch/regs-gpio.h>
#include <mach/regs-gpio.h>
//#include <asm/hardware.h>
#include <mach/hardware.h>
#include <linux/device.h>
#include <linux/gpio.h>

#define DQ_PIN       S3C2410_GPB(5)
#define DQ_PIN_OUTP  S3C2410_GPIO_OUTPUT
#define DQ_PIN_INP   S3C2410_GPIO_INPUT
#define HIGH 1
#define LOW 0

//定义DS18B20ROM指令
#define    DS18B20_ReadROM        0x33    //读ROM
#define    DS18B20_MatchROM       0x55    //匹配ROM
#define    DS18B20_SkipROM        0xCC    //跳过ROM
#define    DS18B20_SearchROM      0xF0    //搜索ROM
#define    DS18B20_AlarmROM       0xEC    //报警搜索

//定义DS18B20存储器操作命令
#define    DS18B20_WriteSCR       0x4E    //写暂存存储器
#define    DS18B20_ReadSCR        0xBE    //读暂存存储器
#define    DS18B20_CopySCR        0x48    //复制暂存存储器
#define    DS18B20_ConvertTemp    0x44    //温度变换
#define    DS18B20_RecallEP       0xB8    //重新调出
#define    DS18B20_ReadPower      0xB4    //读电源

typedef unsigned char U8;
typedef unsigned short U16;

#define DEVICE_NAME	"TX2440-ds18b20"    /* 设备名称 */		
static int DS18B20_Major = 0;			/* 主设备号 */

U16 temp;
U8 DS18B20_ID[8] = {0};

U8 DS18B20_Init(void)
{	
	s3c2410_gpio_cfgpin(DQ_PIN, DQ_PIN_OUTP);
	s3c2410_gpio_setpin(DQ_PIN, LOW);
	udelay(700);
	s3c2410_gpio_setpin(DQ_PIN, HIGH);
	udelay(4);
	s3c2410_gpio_cfgpin(DQ_PIN, DQ_PIN_INP);
	udelay(100);
	
/*	if(s3c2410_gpio_getpin(DQ_PIN)!=0)	//测试复位是否成功
	{
		printk("There are no 18B20 at GPB5! 0x%x\n");
		return 1;
	}
	else
		printk("Init 18B20 succeed!\n");
*/
	return 0;
}

U8 DS18B20_ReadBit(void)    
{
	U8 dat;
	s3c2410_gpio_cfgpin(DQ_PIN, DQ_PIN_OUTP);
	s3c2410_gpio_setpin(DQ_PIN, LOW);
	udelay(2);
	s3c2410_gpio_setpin(DQ_PIN, HIGH); //add
	s3c2410_gpio_cfgpin(DQ_PIN, DQ_PIN_INP);
	udelay(10);
	if(s3c2410_gpio_getpin(DQ_PIN)!=0)
		dat=1;
	else 
		dat=0;
	s3c2410_gpio_cfgpin(DQ_PIN, DQ_PIN_OUTP);
	s3c2410_gpio_setpin(DQ_PIN, HIGH);//add
	udelay(50);
	return (dat);
}

U8 DS18B20_ReadByte(void)   
{
	U8 i,j,dat;
	dat=0;
	for(i=1;i<=8;i++)
	{
		j=DS18B20_ReadBit();
		dat=(j<<7)|(dat>>1);   //读出的数据最低位在最前面，这样刚好一个字节在DAT里
	}
	return(dat);
}

void DS18B20_WriteByte(U8 dat) 
{
	U8 j;
	U8 testb;
	s3c2410_gpio_cfgpin(DQ_PIN, DQ_PIN_OUTP);
	for(j=1;j<=8;j++)
	{
		testb=dat&0x01;
		dat=dat>>1;
		if(testb)     
		{
			s3c2410_gpio_setpin(DQ_PIN, LOW);
			udelay(8);
			s3c2410_gpio_setpin(DQ_PIN, HIGH);
			udelay(50);
		}
		else
		{
			s3c2410_gpio_setpin(DQ_PIN, LOW);
			udelay(90);
			s3c2410_gpio_setpin(DQ_PIN, HIGH);
			udelay(8);

		}
	}
}

void DS18B20_ReadID(void)
{
    udelay(1);
    DS18B20_Init();
    DS18B20_WriteByte(DS18B20_ReadROM);
    DS18B20_ID[0] = DS18B20_ReadByte();
    DS18B20_ID[1] = DS18B20_ReadByte();
    DS18B20_ID[2] = DS18B20_ReadByte();
    DS18B20_ID[3] = DS18B20_ReadByte();
    DS18B20_ID[4] = DS18B20_ReadByte();
    DS18B20_ID[5] = DS18B20_ReadByte();
    DS18B20_ID[6] = DS18B20_ReadByte();
    DS18B20_ID[7] = DS18B20_ReadByte();
}

void DS18B20_Match(void)
{
    DS18B20_WriteByte(DS18B20_MatchROM);
    DS18B20_WriteByte(DS18B20_ID[0]);
    DS18B20_WriteByte(DS18B20_ID[1]);
    DS18B20_WriteByte(DS18B20_ID[2]);
    DS18B20_WriteByte(DS18B20_ID[3]);
    DS18B20_WriteByte(DS18B20_ID[4]);
    DS18B20_WriteByte(DS18B20_ID[5]);
    DS18B20_WriteByte(DS18B20_ID[6]);
    DS18B20_WriteByte(DS18B20_ID[7]);
}

void DS18B20_TmpChange(void)
{
	while(DS18B20_Init());
	udelay(100);/*delay(1);*/
	DS18B20_WriteByte(DS18B20_SkipROM);  // address all drivers on bus
	DS18B20_WriteByte(DS18B20_ConvertTemp);  //  initiates a single temperature conversion
}

U16 DS18B20_Temperature(void)
{
	U8 a,b;
	while(DS18B20_Init());
	udelay(100);
	DS18B20_WriteByte(DS18B20_SkipROM);
	DS18B20_WriteByte(DS18B20_ReadSCR);
	udelay(4);
	a=DS18B20_ReadByte();
	b=DS18B20_ReadByte();
	temp=b;
	temp<<=8;             //two byte  compose a int variable
	temp=temp|a;

	return temp;
}

static int TX2440_ds18b20_open(struct inode *inode, struct file *file)
{
	printk("TX2440-DS18B20 Driver Open is Called!\n");
	DS18B20_TmpChange();
	mdelay(1000);
	DS18B20_Temperature();
	DS18B20_TmpChange();
	mdelay(1000);
	DS18B20_Temperature();
	return 0;
}

static int TX2440_ds18b20_release(struct inode *inode, struct file *file)
{
	printk("TX2440-DS18B20 Driver Release Called!\n");
	return 0;
}

static int TX2440_ds18b20_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	U16 ds18b20_val;
	
	DS18B20_TmpChange();
	mdelay(500);
	ds18b20_val =  DS18B20_Temperature();
	if (ds18b20_val < 10000)
		copy_to_user(buf, &ds18b20_val, sizeof(ds18b20_val));
	
	return 0;
}

static struct file_operations TX2440_ds18b20_fops =
{
	.owner  =   THIS_MODULE,
	.open   =   TX2440_ds18b20_open, 
	.release =  TX2440_ds18b20_release,
//	.ioctl  =   TX2440_ds18b20_ioctl,
	.read   =   TX2440_ds18b20_read,
};

static struct class *ds18b20_class;

static int __init TX2440_ds18b20_init(void)
{

	printk("TX2440 DS18B20 DRIVER MODULE INIT\n");

	DS18B20_Init();  //DS18B20初始化

	DS18B20_Major = register_chrdev(0, DEVICE_NAME, &TX2440_ds18b20_fops);
	if (DS18B20_Major < 0)
	{
		printk(DEVICE_NAME " can't register major number\n");
		return DS18B20_Major;
	}
	printk("register TX2440-DS18B20 Driver OK! Major = %d\n", DS18B20_Major);

	ds18b20_class = class_create(THIS_MODULE, DEVICE_NAME);
	if(IS_ERR(ds18b20_class))
	{
		printk("Err: failed in TX2440-DS18B20 class. \n");
		return -1;
	}

	device_create(ds18b20_class, NULL, MKDEV(DS18B20_Major, 0), NULL, DEVICE_NAME);

	printk(DEVICE_NAME " initialized\n");
	return 0;
}

static void __exit TX2440_ds18b20_exit(void)
{
	printk("TX2440 DS18B20 DRIVER MODULE EXIT\n");
	unregister_chrdev(DS18B20_Major, DEVICE_NAME);
	device_destroy(ds18b20_class, MKDEV(DS18B20_Major, 0));
	class_destroy(ds18b20_class);
}

module_init(TX2440_ds18b20_init);
module_exit(TX2440_ds18b20_exit);

MODULE_AUTHOR("www.txmcu.com");		
MODULE_DESCRIPTION("TX2440 DS18B20 Driver");	
MODULE_LICENSE("GPL");
