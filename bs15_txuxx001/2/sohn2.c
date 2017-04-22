#include <stdio.h>
#include <stdlib.h>

#define MAX_COUNT 300000000

int main(int argc, char *argv[])
{
	printf("Sohn_2 pid : %d, father's pid: %d \n", getpid(), getppid());
	int i=0;
	while(i<10){
		int c = 0;
		while(c < MAX_COUNT)
			c++;
		printf(".\n");
		i++;
	}
	printf("son2 finish	date: \n");
	system("date");
	return 0;

}
