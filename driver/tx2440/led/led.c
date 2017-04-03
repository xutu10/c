/******************************************************
LED�������Գ�����TX2440A������������

�����÷���
		�豸���ƣ�TX2440-led
		���Գ������ƣ�led
		���ֻ��һ���������Ҳ���Ϊ0 1
		led 0���ĸ���ȫ��
		led 1: �ĸ���ȫ��
		�������������������1��ʾ��յ�ƣ�����2��ʾ����
		led 1 0����һյ����
		led 4 1: ����յ����
*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	unsigned int on;
	unsigned int led_num;
	int fd;
	fd = open("/dev/TX2440-led", 0);
	if (fd < 0)
	{
		perror("open device led");
		exit(1);
	}

	if (argc == 2)
	{
		sscanf(argv[1], "%d", &on);
		if (on < 2)
		{
			ioctl(fd, on+3, 0);
		}
		else
		{
			printf("Usage: led led_num 0|1  or  led 0|1\n");
			exit(1);
		}
	}
	
	if (argc == 3)
	{
		sscanf(argv[1], "%d", &led_num);
		sscanf(argv[2], "%d", &on);
		if ((on < 2) && (led_num>0 || led_num < 4))
			ioctl(fd, on, (led_num-1));
		else
		{
			printf("Usage: led led_num 0|1  or  led 0|1\n");
			exit(1);
		}
	}

	close(fd);
	return 0;
}

