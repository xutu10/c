/******************************************************
按键驱动测试程序，在TX2440A开发板做测试

驱动用法：
		设备名称：TX2440-button
		测试程序名称：key
*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

int main(void)
{
	int i;
	int key_fd;
	int key_value[4];

	key_fd = open("/dev/TX2440-button", 0);
	if (key_fd < 0)
	{
		perror("open device buttons");
		exit(1);
	}

	while(1)
	{
		int len;
		len = read(key_fd, key_value, sizeof(key_value));
		if (len != sizeof(key_value))
		{
			perror("read button\n");
			continue;
		}
		
		else
		{
			for (i = 0; i < 4; i++)
			{
				if(key_value[i] != 0)
					printf("K%d pressed, key value = 0x%02x\n", i+1, key_value[i]);
			}
		}
	}
	
	close(key_fd);
	return 0;
}
