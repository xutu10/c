#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define ITERATIONS 6
#define SLEEP_TIME 2

int main(int argc, char *argv[])
{
	int son1 = fork();
	int son2 =0;
	if(son1 > 0)
		son2 = fork();

	if(son1>0 && son2 > 0){
		printf("Vater: %d, first child : %d , second child %d\n", getpid(),son1, son2);
		/*printf("start to wait for childprocess\n");
		while(-1 != wait(NULL)){}							// at this position, after two childprocesses finish,then continue.
*/
		int i=0;
		while(i < ITERATIONS){
			sleep(SLEEP_TIME);
			system("date");
			system("ps au");
			i++;
		}
		char sys[255];
		sprintf(sys,"pstree -h %d",getpid());
		system(sys);		
		printf("start to wait for childprocess\n");     
		while(-1 != wait(NULL)){}
		printf("father finish date:\n");
		system("date");
		
	}

	if(0 == son2 && 0 == son1){
		printf("son1 \n");
		char *arg[] = {NULL};
		if(-1 == execve("./sohn1",arg,NULL))
			printf("error\n");				
	}

	if(0 == son2 && son1 > 0){
		printf("son2\n");
		char *arg[] = {NULL};
		if(-1 == execve("./sohn2",arg,NULL))
			printf("error\n");
	}
	return 0;
}
