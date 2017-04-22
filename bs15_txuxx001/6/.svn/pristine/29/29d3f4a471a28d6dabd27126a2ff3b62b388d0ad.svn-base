#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "shared.h"
#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

#define ITERATION 10

void msleep(unsigned int ms)
{
	struct timespec ts;

	assert(ms > 0);
	assert(ms < 1000);

	ts.tv_sec = 0;
	ts.tv_nsec = ms * 1000 * 1000;
	nanosleep(&ts, NULL);
}

int main(int argc, char *argv[])
{
	printf("schreiber.c\n");
	char* shm_name = argv[1];
	char* zeichen = argv[2];
	int zeichen_laenge = atoi(argv[3]);
	int sleep_time = atoi(argv[4]);
	// open the created share memory
	int fd = shm_open(shm_name,O_RDWR,0);
	if(fd < 0){
		printf("shm open failed\n");
		return -1;
	}
	// set the mapping between the process and share memory
	void* p = mmap(NULL, 500,PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
	// cast to pointer of semaphore
	sem_t* s = (sem_t*)p;
	// get the pointer of struct shared
	struct shared* message = (struct shared*)(s+1);
	int i,j;
	char* payload;
	for(i=0;i<ITERATION;i++){
		// lock the critical region
		int ret = sem_wait(s);
		if(ret < 0){
			printf("semaphore wait failed\n");
			return -1;
		}
		//getchar();??
		message-> pid = getpid();
		message-> i = i;
		message-> length = zeichen_laenge;
		// get the pointer of payload 
		payload = (char*)(message + 1);
		// write into payload
		for(j=0;j<zeichen_laenge;j++){
			payload[j] = *zeichen;
			msleep(sleep_time);
		}
		// unlock the critical region
		ret = sem_post(s);
		if(ret < 0){
			printf("semaphore post failed\n");
			return -1;
		}
		msleep(sleep_time);
	}
	return 0;

}
