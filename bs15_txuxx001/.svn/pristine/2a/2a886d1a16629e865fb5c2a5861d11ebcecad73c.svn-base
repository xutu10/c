#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

#define RUNDEN 10
#define N 5
#define LEFT (i+N-1)%N
#define RIGHT (i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;		//init mutex
pthread_cond_t philosophenCond[N];						//init condition variable

void thinking(int i){
	
	srand(time(NULL));
	int time = ((double)rand()/RAND_MAX)*10;			// wait for random time 
	sleep(time);
}

void eating(int i){
	srand(time(NULL));
	int time = ((double)rand()/RAND_MAX)*10;
	printf("state: Philosopher 0:%d Philosopher 1:%d Philosopher 2:%d Philosopher 3:%d Philosopher 4:%d\n",
		state[0],state[1], state[2],state[3],state[4] );			//output the present state of all of philosophers
	sleep(time);
}

void test(int i){
	int ret;
	if(state[i] == HUNGRY && state[LEFT] != EATING &&				// when meet the condition, then eat
		state[RIGHT] != EATING){
		state[i] = EATING;
		ret = pthread_cond_signal(&philosophenCond[(i)%N]);			// send the signal to someone, who can eat now
		assert(0 == ret);
	}
}

void take_forks(int i){
	int ret;
	ret = pthread_mutex_lock(&mutex);					//lock the critical region
	assert(0 == ret);
	state[i] = HUNGRY;	
	test(i);
	while (state[i] != EATING) 
		pthread_cond_wait(&philosophenCond[i],&mutex);    // as cannot get fork, waiting and unlock
	ret = pthread_mutex_unlock(&mutex);
	assert(0 == ret);	
}														

void put_forks(int i){
	int ret;
	ret = pthread_mutex_lock(&mutex);					// lock mutex
	assert(0 == ret);
	state[i] = THINKING;
	test(LEFT);								//check, if the left one can eat
	test(RIGHT);								//check, if the right one can eat
	ret = pthread_mutex_unlock(&mutex);					// unlock mutex
	assert(0 == ret);
}

void* start(void* arg){	
	int i = (int)arg;
	int j;
	for(j=0;j<RUNDEN;j++){
		thinking(i);											
		take_forks(i);						// try to take forks
		eating(i);												
		put_forks(i);						// put forks
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	printf("Dinierende Philosophen ...\n\n");
	pthread_t philosopher[N];					//create threads
	pthread_attr_t attr[N];						// create attr
	int ret,i=0;
	
	for(i=0;i<N;i++){
		ret = pthread_attr_init(&attr[i]);				//init attr
		assert(0 == ret);		
		ret = pthread_create(&philosopher[i],&attr[i],start,(void*)i);  //init threads
		assert(0 == ret);
	}

	for(i=0;i<N;i++){
		ret = pthread_join(philosopher[i],NULL);			// wait for a pthread until he finished
		assert(0 == ret);
	}

	for(i=0;i<N;i++){
		ret = pthread_cond_destroy(&philosophenCond[i]);    // destroy the condition variables
		assert(0 == ret);
	}

	ret = pthread_mutex_destroy(&mutex);					//destroy mutex
	assert(0 == ret);
	printf("Abspuelen nicht vergessen!\n");

	return 0;
}
