/******************************************************
GPSģ���������Գ�����TX2440A������������

�����÷���
	�豸���ƣ�TX2440-gps
	���Գ������ƣ�gps
	ʹ�õĴ��ڣ�s3c2410_serial2

	���в��Գ��򣬽�GPSģ�����TX2440A�������Ϸ���GPS�ӿ��ϣ�5P��ɫ����
	���ն��ϴ�ӡ�����յ�����
	
ά����¼��
	2010-06-11    V1.0    
*******************************************************/
#include <stdio.h>      /*��׼�����������*/
#include <stdlib.h>     /*��׼�����ⶨ��*/
#include <unistd.h>     /*Unix ��׼��������*/
#include <sys/types.h>  
#include <sys/stat.h> 
#include <fcntl.h>      /*�ļ����ƶ���*/
#include <termios.h>    /*PPSIX �ն˿��ƶ���*/
#include <errno.h>      /*����Ŷ���*/
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
	switch (databits) /*��������λ��*/
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
			options.c_cflag |= (PARODD | PARENB); /* ����Ϊ��Ч��*/  
			options.c_iflag |= INPCK;             /* Disnable parity checking */ 
			break;  
		case 'e':  
		case 'E':   
			options.c_cflag |= PARENB;     /* Enable parity */    
			options.c_cflag &= ~PARODD;   /* ת��ΪżЧ��*/     
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
	/* ����ֹͣλ*/  
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
	
	options.c_cc[VTIME] = 50; /* ���ó�ʱ5 seconds*/   
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
	
	options.c_cflag |= (CLOCAL | CREAD);  //����DCD�ź�
	options.c_cflag &= ~CRTSCTS;          //��Ӳ��������
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //�ػ���
	options.c_oflag &= ~OPOST;	                        //��ʽ���
	options.c_iflag &= ~(IXON | IXOFF | IXANY);         //�����������
			
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
	char *dev  = "/dev/s3c2410_serial2"; //���ڶ�
	
	if (gps_fd = open("/dev/TX2440-gps", 0) < 0)		
	{ 			
		perror("open device gps");
		exit (0);		
	}
		
	uart_fd = OpenDev(dev);	//���豸	
	set_speed(uart_fd, 9600); //���ò�����	
	
	if (set_Parity(uart_fd,8,1,'n') == FALSE)  //����λ��ֹͣλ��У��λ
	{
		printf("Set Parity Error\n");
		exit (0);
	}
	
	while(nread = read(uart_fd, rev_char, 1))  //������ֵ
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

