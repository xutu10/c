#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define NUM_THREADS 6
#define ITERATIONS 5
#define ANZ_ZEICHEN 10

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
	int i;
	for(i=0;i<ITERATIONS;i++){						// output 5 times
		int j;
		for(j=0;j<ANZ_ZEICHEN;j++){					//output 10 letters in one row with out the critical region
			printf("%c", out);								
			srand(time(NULL));
			int time = ((double)rand()/RAND_MAX)*400+100;
			msleep(time);
			fflush(stdout);
		}
		printf("\n");
	}

	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	
	pthread_t thread[NUM_THREADS];					//create threads
	pthread_attr_t attr[NUM_THREADS];
	char zeichen[6] = {'a','b','c','d','f','e'};
	int ret;
	int i;
	for(i=0;i<NUM_THREADS;i++){
		ret = pthread_attr_init(&attr[i]);			//init atrr
		assert(0 == ret);

		ret = pthread_create(&thread[i],&attr[i],thread_entry,&zeichen[i]);		//init threads
		assert(0 == ret);
	}

	for(i=0;i<NUM_THREADS;i++){
		ret = pthread_join(thread[i],NULL);						//wait until all of threads finish
		assert(0 == ret);
	}


	return 0;
}