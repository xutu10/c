#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO "myfifo"
#define BUFF_SIZE 1024

int main(int argc, char const *argv[])
{
	printf("verbr1.c \n");
	char buff[BUFF_SIZE];
	int ret, fd;

	if(-1 == access(FIFO,F_OK)){
		if(mkfifo(FIFO,0666) < 0){
			printf("can not create fifo file\n");
			exit(1);
		}			
	}
	// open the pipe for reading
	if(-1 == (fd = open(FIFO,O_RDONLY))){
		printf("open file failed\n");
		exit(1);
	}	
	while(1){
		memset(buff,0,BUFF_SIZE);
		//read the message from pipe
		if((ret = read(fd,buff,BUFF_SIZE)) <= 0){
			printf("read fifo failed \n" );
			exit(1);
		}		
		// output the message
		printf("read from pipe: %s\n", buff);
	}
	printf("finish\n");
	close(fd);
	return 0;
}
