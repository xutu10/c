/******************************************************
ADC�������Գ�����TX2440A������������

�����÷���
		�豸���ƣ�TX2440-adc
		���Գ������ƣ�adc
		adc���������0 or 1 ѡ��ADͨ��
*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	int ret;
	int data, ch;
	
	fd = open("/dev/TX2440-adc", 0);
	if (fd < 0)
	{
		perror("open device adc");
		exit(1);
	}
	
	sscanf(argv[1], "%d", &ch);
	if ((argc < 3) && (ch < 2))
	{
		printf("ch=%d\n", ch);
		ret = ioctl(fd, ch, 0);
		if(ret<0)
            perror("ioctl set adc channel error");
	}
	else
	{
		printf("Usage: adc 0 or 1\n");
		exit(1);
	}
	while(1)
	{
		ret = read(fd,&data,4);
		if(ret<0)
				printf("read adc error\n");
		printf("adc is %d\n",data);
		sleep(1);	
	}
	
	close(fd);
	return 0;
}
