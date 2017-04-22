#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

#define ALARM_PERIOD 4

void new_op(int signum, siginfo_t *info, void *myact){

	if(signum == SIGUSR1)
		printf("Singal SIGUSR1 emfangen \n");
	else if(signum == SIGUSR2)
		printf("Singal SIGUSR2 emfangen \n");
	else if(signum == SIGALRM){
		printf("Timer abgelaufen \n");
		alarm(ALARM_PERIOD);
	}
	else if(signum == SIGTERM){
		printf("Programmende \n");
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	printf("sig_empf1.c\n");

	struct sigaction act;
	//set the mask empty
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = new_op;

	// register sigaction with SIGUSR1
	if(sigaction(SIGUSR1,&act,NULL) < 0)
		printf("register singal error\n");
	// register sigaction with SIGUSR2
	if(sigaction(SIGUSR2,&act,NULL) < 0)
		printf("register singal error\n");
	// register sigaction with SIGTERM
	if(sigaction(SIGTERM,&act,NULL) < 0)
		printf("register singal error\n");
	// register sigaction with SIGALRM
	if(sigaction(SIGALRM,&act,NULL) < 0)
		printf("register singal error\n");
	
	alarm(ALARM_PERIOD);

	while(1)
		sleep(2);

	return 0;
}