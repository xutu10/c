#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h>
#include <semaphore.h>


int main(int argc, char *argv[])
{
	printf("leser.c\n");
	char* shm_name = argv[1];
	char* payload;
	// open the created share memory
	int fd = shm_open(shm_name,O_RDWR,0);
	if(fd < 0){
		printf("shm open failed\n");
		return -1;
	}
	// set the mapping between the process and share memory
	void* p = mmap(NULL,500,PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
	// get the pointer of struct shared
	struct shared* message = (struct shared*)((sem_t*)p+1);

	int j,i;
	for(j=0;j<20;j++){
		getchar();
		printf("%d %d: ",message-> pid, message-> i);	
		// get the pointer of payload 	
		payload = (char*)(message + 1);
		for(i=0;i< message-> length;i++){
			// read the payload 
			printf("%c",payload[i] );
		}
		printf("\n");
	}
	return 0;
}
