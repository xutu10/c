/******************************************************
DS18B20驱动测试程序，在TX2440A开发板做测试

驱动用法：
		设备名称：TX2440-ds18b20
		测试程序名称：ds18b20
*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	int ret;
	float temp;
	unsigned short data;
	
	fd = open("/dev/TX2440-ds18b20", 0);
	if (fd < 0)
	{
		perror("open device ds18b20");
		exit(1);
	}

	while(1)
	{
		ret = read(fd,&data,4);
		if(ret<0)
				printf("read ds18b20 error\n");
		
		temp = data * 0.0625;
		data = temp * 10 + 0.5;
		
		printf("Temperature is  [%d.%d `C]\n", data/10, data%10);
		sleep(1);	
	}
	
	close(fd);
	return 0;
}
