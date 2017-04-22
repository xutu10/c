#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define FIFO "myfifo"
#define BUFF_SIZE 1024
#define ITERATION 20
pthread_mutex_t mutex;
int i = 0;    			// i is counter for iteration
int sleep1,sleep2;	

void* thread_entry(void* arg){

	int thread = (int) arg; 
	int fd,ret,sleep1=0,sleep2 = 0;
	char buff[BUFF_SIZE];		
	while(i<ITERATION){	
		if(-1 == (fd = open(FIFO,O_WRONLY))){
			printf("open file failed\n");
			exit(1);
		}		
		// lock the critical region
		ret = pthread_mutex_lock(&mutex); 
		if(0 != ret)
			printf("lock mutex failed\n");
		
		memset(buff,0,BUFF_SIZE);
		sprintf(buff,"%d",thread);
		// write thread nummber into pine
		if((ret = write(fd,buff,BUFF_SIZE)) < 0 )
			printf("write failed\n");
		sleep(sleep2);

		memset(buff,0,BUFF_SIZE);		
		sprintf(buff,":%d:",i); 
		// write the counter into pipe
		if((ret = write(fd,buff,BUFF_SIZE)) < 0 )
			printf("write failed\n");
		sleep(sleep2);

		memset(buff,0,BUFF_SIZE);
		sprintf(buff,"i am thread %d", thread);
		// write the p into pipe
		if((ret = write(fd,buff,BUFF_SIZE)) < 0 )
			printf("write failed\n");
		sleep(sleep2);
		// unlock the critical region
		ret = pthread_mutex_unlock(&mutex); 
		if(0 != ret)
			printf("unlock mutex failed\n");
		i++;
		sleep(sleep1);
		close(fd);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	printf("erzeug2.c\n");
	int ret = 0;
	int nummer,j;
	nummer = atoi(argv[1]);
	sleep1 = atoi(argv[2]);
	sleep2 = atoi(argv[3]);
	
	//create threads according to the nummer from user
	pthread_t thread[nummer];		
	pthread_attr_t attr[nummer];
	ret = pthread_mutex_init(&mutex,NULL);
	if(0 != ret)
			printf("init mutex failed \n");
	// check if pipe alread exist
	if(-1 == access(FIFO,F_OK)){
		if(mkfifo(FIFO,0666) < 0){
			printf("can not create fifo file\n");
			exit(1);
		}			
	}
	for(j=0;j<nummer;j++){		
		//init atrr
		if( 0 != (ret = pthread_attr_init(&attr[j])))			
			printf("init pthread failed\n");
		ret = pthread_create(&thread[j],&attr[j],thread_entry,(void*)j); 
		if(0 != ret)
			printf("create pthread failed \n");
	}
	//wait until all of threads finish
	for(j=0;j<nummer;j++){
		ret = pthread_join(thread[j],NULL);		
		if(0 != ret)
			printf("join pthread failed \n");
	}
	pthread_mutex_destroy(&mutex);		
	printf("finish\n");
	
	return 0;
}
