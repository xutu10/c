/*************************************
PWM�������Գ�����TX2440A������������

�����÷���
		�豸���ƣ�TX2440-Beep
		���Գ������ƣ�beep
		����Ϊ��������Ƶ��
*************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd;
	unsigned long temp =0;
	int i;

	fd = open("/dev/TX2440-Beep", O_RDWR);
	if (fd < 0)
	{
		perror("open device TX2440-BEEP");
		exit(1);
	}
	printf("Please enter the times number (0 is stop) :\n");

	while(1)
	{
		scanf("%d",&temp);
		printf("times = %d \n",temp);
		ioctl(fd, temp, 4);
		if(temp == 0)
		{
			printf("Stop Control Beep!\n");
			break;
		}
	}
	close(fd);
	return 0;
}

