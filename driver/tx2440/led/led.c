/******************************************************
LED驱动测试程序，在TX2440A开发板做测试

驱动用法：
		设备名称：TX2440-led
		测试程序名称：led
		如果只带一个参数，且参数为0 1
		led 0：四个灯全灭
		led 1: 四个灯全亮
		如果带两个参数，参数1表示哪盏灯，参数2表示亮灭
		led 1 0：第一盏灯灭
		led 4 1: 第四盏灯亮
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

