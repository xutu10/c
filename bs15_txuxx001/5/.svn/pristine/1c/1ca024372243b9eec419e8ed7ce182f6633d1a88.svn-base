#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO "myfifo"
#define BUFF_SIZE 1024
#define ITERATION 5

int main(int argc, char const *argv[])
{
	printf("erzeug1.c \n");
	char buff[BUFF_SIZE];
	int ret, fd,pid,i= 0;
	char counter[2];

	if(-1 == access(FIFO,F_OK)){
		if(mkfifo(FIFO,0666) < 0){
			printf("can not create fifo file\n");
			exit(1);
		}			
	}
	// open the pipe for writing
	if(-1 == (fd = open(FIFO,O_WRONLY))){
		printf("open file failed\n");
		exit(1);
	}
	// get the pid
	pid = getpid();

	for(i=0;i<ITERATION;i++){
		memset(buff,0,BUFF_SIZE);
		// add pid into buff
		sprintf(buff,"%d: ",pid);
		//transfer int to string
		sprintf(counter,"%d",i);  
		// append counter i to buff
		strcat(buff,counter);
		// write buff into pipe
		if((ret = write(fd,buff,10)) <= 0){
			printf("write into fifo failed \n" );
			exit(1);
		}
		sleep(2);
	}
	printf("finish\n");
	close(fd);
	return 0;
}