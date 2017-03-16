#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int fd;
	fd = open("/dev/XXXX", 0);
	if (fd < 0)
	{
		perror("open device");
		exit(1);
	}

	/*your code*/

	close(fd);
	return 0;
}

