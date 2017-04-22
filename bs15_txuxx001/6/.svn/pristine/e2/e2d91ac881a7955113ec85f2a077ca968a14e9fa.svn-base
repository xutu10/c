#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

#define RUNDEN 10
#define N 5
#define LEFT (i+N-1)%N
#define RIGHT (i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2


struct shared_state* p_map;

/* Die Struktur der im SHM abgelegten Daten */
struct shared_state {
	int state[N];
	sem_t s[N+1];
};

void thinking(int i){	
	sleep(1); // wait for 1 seconde 
}

void eating(int i){
	sleep(1);
	
}

void test(int i){	
	int ret;
	if(p_map-> state[i] == HUNGRY && p_map-> state[LEFT] != EATING &&				// when meet the condition, then eat
		p_map-> state[RIGHT] != EATING){
		p_map-> state[i] = EATING;		
		ret = sem_post(&(p_map->s[i]));						// unlock the one, who can eat now						
		if( ret != 0){
			printf("semaphore post failure\n");
			exit(-1);
		}
		//output the present state of all of philosophers
		printf("state: 0.Philosopher:%d,  1.Philosopher:%d,  2.Philosopher:%d,  3.Philosopher:%d,  4.Philosopher:%d\n",
			p_map-> state[0],p_map-> state[1], p_map-> state[2],p_map-> state[3], p_map-> state[4]);
	}
}

void take_forks(int i){
	int ret;	
	ret = sem_wait(&(p_map->s[N]));							//lock the critical region
	if( ret != 0){
		printf("semaphore wait failure\n");
		exit(-1);
	}	
	p_map-> state[i] = HUNGRY;		
	test(i);	
	ret = sem_post(&(p_map->s[N]));					//unlock the critical region
	if( ret != 0){
		printf("semaphore post failure\n");
		exit(-1);
	}
	ret = sem_wait(&(p_map->s[i]));								//lock the one, who cannot eat must wait
	if( ret != 0){
		printf("semaphore wait failure\n");
		exit(-1);
	}  
}														

void put_forks(int i){
	int ret;
	ret = sem_wait(&(p_map->s[N]));								//lock the critical region
	if( ret != 0){
		printf("semaphore wait failure\n");
		exit(-1);
	}
	p_map-> state[i] = THINKING;	
	
	test(LEFT);											//check, if the left one can eat
	test(RIGHT);										//check, if the right one can eat
	ret = sem_post(&(p_map->s[N]));								//lock the critical region
	if( ret != 0){
		printf("semaphore post failure\n");
		exit(-1);
	}
}

void philosopher(int p){	
	int i = p;
	int j;
	for(j=0;j<RUNDEN;j++){
		thinking(i);											
		take_forks(i);									// try to take forks
		eating(i);												
		put_forks(i);									// put forks
	}
	 exit(0);
}

int main(int argc, char *argv[])
{
	printf("Dinierende Philosophen mit Prozessen ...\n\n");
	printf("----1:thinking 2:eating 3:hungry----\n\n");
	p_map = (struct shared_state*)mmap(NULL,sizeof(struct shared_state),
				PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);

	int ret,i;
	// init semaphore, which to be putted in share memory 
	for(i=0;i<N+1;i++){
		ret = sem_init(&(p_map->s[i]),1,1); 
		if(0 != ret){
			printf("semaphore init failure\n");
			return -1;
		}
	}
	// create childprocesses
	for(i=0;i<N;i++){
		ret = fork();
		if(-1 == ret || 0 == ret) break;
	}
	if(-1 == ret){
		printf("fork failure\n");
		exit(-1);
	}
	// for childprocess
	else if(0 == ret){
		philosopher(i);
	}		
	else{ // for parentprocess
		// waiting for all of childprocesses
		while(-1 != wait(NULL)){}
		for(i=0;i<N+1;i++){
		ret = sem_destroy(&(p_map->s[i]));
			if(0 != ret){
				printf("semaphore destroy failure\n");
				return -1;
			}
		}
		printf("\n\nalle Philosophen haben schon gegessen. 88 \n");
	}
	return 0;
}
