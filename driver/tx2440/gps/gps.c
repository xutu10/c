/******************************************************
GPS模块驱动测试程序，在TX2440A开发板做测试

驱动用法：
	设备名称：TX2440-gps
	测试程序名称：gps
	使用的串口：s3c2410_serial2

	运行测试程序，将GPS模块接在TX2440A开发板上方的GPS接口上（5P白色座）
	在终端上打印出接收的数据
	
维护记录：
	2010-06-11    V1.0    
*******************************************************/
#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>  
#include <sys/stat.h> 
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <string.h>

#define FALSE  -1
#define TRUE   0

int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300, };

unsigned char SetBaud_115200[14];

void set_speed(int fd, int speed)
{
	int   i; 
	int   status; 
	struct termios   Opt;
	tcgetattr(fd, &Opt); 
	
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
	{ 
		if  (speed == name_arr[i])
		{     
			tcflush(fd, TCIOFLUSH);     
			cfsetispeed(&Opt, speed_arr[i]);  
			cfsetospeed(&Opt, speed_arr[i]); 
			//printf("set baud:%d\n", name_arr[i]);			
			status = tcsetattr(fd, TCSANOW, &Opt);  
			if  (status != 0)
			{        
				perror("tcsetattr fd");  
				return;     
			}    
			tcflush(fd,TCIOFLUSH);   
		}  
	}
}

int set_Parity(int fd,int databits,int stopbits,int parity)
{ 
	struct termios options; 
	if  ( tcgetattr( fd,&options)  !=  0)
	{ 
		perror("SetupSerial 1");     
		return(FALSE);  
	}
	options.c_cflag &= ~CSIZE; 
	switch (databits) /*设置数据位数*/
	{   
		case 7:		
			options.c_cflag |= CS7; 
			break;
		case 8:     
			options.c_cflag |= CS8;
			break;   
		default:    
			fprintf(stderr,"Unsupported data size\n"); return (FALSE);  
	}
	switch (parity) 
	{   
		case 'n':
		case 'N':    
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
			break;  
		case 'o':   
		case 'O':     
			options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/  
			options.c_iflag |= INPCK;             /* Disnable parity checking */ 
			break;  
		case 'e':  
		case 'E':   
			options.c_cflag |= PARENB;     /* Enable parity */    
			options.c_cflag &= ~PARODD;   /* 转换为偶效验*/     
			options.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		case 'S': 
		case 's':  /*as no parity*/   
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;break;  
		default:   
			fprintf(stderr,"Unsupported parity\n");    
			return (FALSE);  
		}  
	/* 设置停止位*/  
	switch (stopbits)
	{   
		case 1:    
			options.c_cflag &= ~CSTOPB;  
			break;  
		case 2:    
			options.c_cflag |= CSTOPB;  
		   break;
		default:    
			 fprintf(stderr,"Unsupported stop bits\n");  
			 return (FALSE); 
	} 
	/* Set input parity option */ 
	if (parity != 'n')   
		options.c_iflag |= INPCK; 
	tcflush(fd,TCIOFLUSH);
	
	options.c_cc[VTIME] = 50; /* 设置超时5 seconds*/   
	options.c_cc[VMIN] = 0; /* Update the options and do it NOW */	
	options.c_cc[VINTR]    = 0;       /**//* Ctrl-c */
	options.c_cc[VQUIT]     = 0;   /**//* Ctrl- */
	options.c_cc[VERASE]    = 0;   /**//* del */ 
	options.c_cc[VKILL]    = 0;   /**//* @ */ 
	options.c_cc[VEOF]     = 0;   /**//* Ctrl-d */ 
	options.c_cc[VSWTC]    = 0;   /**//* '' */
	options.c_cc[VSTART]   = 0;   /**//* Ctrl-q */ 
	options.c_cc[VSTOP]    = 0;   /**//* Ctrl-s */ 
	options.c_cc[VSUSP]    = 0;   /**//* Ctrl-z */ 
	options.c_cc[VEOL]     = 0;   /**//* '' */ 
	options.c_cc[VREPRINT] = 0;   /**//* Ctrl-r */
	options.c_cc[VDISCARD] = 0;   /**//* Ctrl-u */ 
	options.c_cc[VWERASE]  = 0;   /**//* Ctrl-w */ 
	options.c_cc[VLNEXT]   = 0;   /**//* Ctrl-v */ 
	options.c_cc[VEOL2]    = 0;   /**//* '' */
	
	options.c_cflag |= (CLOCAL | CREAD);  //忽略DCD信号
	options.c_cflag &= ~CRTSCTS;          //关硬件流控制
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //关回显
	options.c_oflag &= ~OPOST;	                        //行式输出
	options.c_iflag &= ~(IXON | IXOFF | IXANY);         //关软件流控制
			
	if (tcsetattr(fd,TCSANOW,&options) != 0)   
	{ 
		perror("SetupSerial 3");   
		return (FALSE);  
	} 
	
	return (TRUE);  
}

int OpenDev(char *Dev)
{
	int	fd = open( Dev, O_RDWR | O_NOCTTY);  //| O_NOCTTY | O_NDELAY	
	if (-1 == fd)	
	{ 			
		perror("Can't Open Serial Port");
		return -1;		
	}	
	else	
		return fd;
}

int main(int argc, char **argv)
{
	int gps_fd, uart_fd;
	int nread, nwrite;
	int num = 0;
	char rev_char[1];
	char rev_buf[128];
	char *dev  = "/dev/s3c2410_serial2"; //串口二
	
	if (gps_fd = open("/dev/TX2440-gps", 0) < 0)		
	{ 			
		perror("open device gps");
		exit (0);		
	}
		
	uart_fd = OpenDev(dev);	//打开设备	
	set_speed(uart_fd, 9600); //设置波特率	
	
	if (set_Parity(uart_fd,8,1,'n') == FALSE)  //数据位，停止位，校验位
	{
		printf("Set Parity Error\n");
		exit (0);
	}
	
	while(nread = read(uart_fd, rev_char, 1))  //读返回值
	{ 
		rev_buf[num++] = rev_char[0];
		if (rev_char[0] == '\n')
		{
			rev_buf[num] = '\0';
			num = 0;
			printf("%s", rev_buf);
		}
	}
	if (nread == 0)
		printf("No GPS Module!\n");
		
	close(uart_fd);
	close(gps_fd);
}

