#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h>
#include <unistd.h>
#include "shared.h"
#include <semaphore.h>



int main(int argc, char *argv[])
{

	char* shm_name = argv[1] ;
	int shm_lenght = atoi(argv[2]);
	sem_t* s; 

	printf("create.c\n\n");	
	// create a share memory object
	int fd = shm_open(shm_name,O_CREAT | O_EXCL | O_RDWR,0666);
	if(fd < 0){
		printf("shm open failed\n");
		return -1;
	}
	// set the size of segement 
	int ret = ftruncate(fd,shm_lenght);
	if( 0 != ret){
		printf("truncate shm failed\n");
		return -1;
	}
	//set the mapping between process and share memory
	void* p = mmap(NULL,500,PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
	s = (sem_t*)p;
	// init semaphore
	ret = sem_init(s,1,1);
	if(ret < 0){
		printf("semaphore init failed\n");
		return -1;
	}
	
	return 0;
}
