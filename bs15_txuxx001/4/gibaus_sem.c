#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_THREADS 6
#define ITERATIONS 5
#define ANZ_ZEICHEN 10

sem_t s;

void msleep(unsigned int ms)
{
	struct timespec ts;

	assert(ms > 0);
	assert(ms < 1000);

	ts.tv_sec = 0;
	ts.tv_nsec = ms * 1000 * 1000;
	nanosleep(&ts, NULL);
}

void* thread_entry(void* arg){
	char out = *((char*)arg);
	int i,ret;
	for(i=0;i<ITERATIONS;i++){
		int j;
		ret = sem_wait(&s);					//lock the critical region		
		assert(0 == ret);
		for(j=0;j<ANZ_ZEICHEN;j++){			// output 10 letters in one row 
			printf("%c", out);				// in critical region with semaphor
			srand(time(NULL));
			int time = ((double)rand()/RAND_MAX)*400+100;
			msleep(time);
			fflush(stdout);
		}
		printf("\n");
		ret = sem_post(&s);					//unlock the critical region
		assert(0 == ret);
	}

	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr[NUM_THREADS];
	char zeichen[6] = {'a','b','c','d','f','e'};
	int ret;
	int i;

	ret = sem_init(&s,0,1);  							// init semaphore
	assert(0 == ret);

	for(i=0;i<NUM_THREADS;i++){
		ret = pthread_attr_init(&attr[i]);
		assert(0 == ret);

		ret = pthread_create(&thread[i],&attr[i],thread_entry,&zeichen[i]);		//init threads
		assert(0 == ret);
	}

	for(i=0;i<NUM_THREADS;i++){
		ret = pthread_join(thread[i],NULL);
		assert(0 == ret);
	}


	return 0;
}