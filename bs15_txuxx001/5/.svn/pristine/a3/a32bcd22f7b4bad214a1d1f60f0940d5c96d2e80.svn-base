#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>

// problem with strtok???
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
	printf("sig_sender.c\n");
	char* input = NULL;
	int sig1 = 0,sig2 = 0 ,pid = 0, wait_time = 0;
	// all of the signals to select
	printf("9 SIGKILL, 10 SIGUSR1, 12 SIGUSR2, 14 SIGALRM, 15 SIGTERM\n");
	printf("--------\n select the signals\n");
	// get the selection from user
	fgets(input,7,stdin);
	// clear stdin
	fflush(stdin);
	char* tmpsig1 ;
	char* tmpsig2 ;
	// split string from blankspace
	tmpsig1 = strtok(input," ");
	tmpsig2 = strtok(NULL," ");
	// transfer pointer to int
	sig1 = atoi(tmpsig1);
	if(NULL != tmpsig2){
		sig2 = atoi(tmpsig2);
		printf("--------\n select warting time \n ");
		fgets(input,7,stdin);
		fflush(stdin);
		wait_time = atoi(input);
		printf("warting: %d\n", wait_time);
	}

	printf("------------\n");
	char* show_allpid = "ps u";
	system(show_allpid);
	printf("----------\n select the pid\n");
	fgets(input, 7,stdin);
	fflush(stdin);	
	pid = atoi(input);

	kill(pid,sig1);
	/*if(0 != ret)
		printf("kill failed\n");
		????
	*/
	if(0 != sig2 ){
		msleep(wait_time);
		kill(pid,sig2);
	}
	
	return 0;
}
