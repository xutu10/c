#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO "myfifo"
#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
	printf("verbr2.c\n");
	char buff[BUFF_SIZE];
	int ret, fd,pid;

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
	int i;
	while(1){
		memset(buff,0,BUFF_SIZE);
		//read the pid from pipe
		if((ret = read(fd,buff,BUFF_SIZE)) <= 0){
			printf("read fifo failed \n" );
			exit(1);
		}	
		pid = atoi(buff);
		// output 5*pid blankspaces before message
		for(i = 0;i<5*pid;i++)
			printf(" ");
		// output the pid 
		printf("pid %s\n", buff);
		
		memset(buff,0,BUFF_SIZE);	
		// read i from pipe
		if((ret = read(fd,buff,BUFF_SIZE)) <= 0){
			printf("read fifo failed \n" );
			exit(1);
		}	
		// output 5*pid blankspaces before message
		for(i = 0;i<5*pid;i++)
			printf(" ");
		// output i
		printf("counter %s\n", buff);

		memset(buff,0,BUFF_SIZE);	
		//read p from pipe
		if((ret = read(fd,buff,BUFF_SIZE)) <= 0){
			printf("read fifo failed \n" );
			exit(1);
		}	
		// output 5*pid blankspaces before message
		for(i = 0;i<5*pid;i++)
			printf(" ");
		// output p
		printf("message %s\n", buff);	
	}

	printf("finish\n");
	close(fd);

	return 0;
}
