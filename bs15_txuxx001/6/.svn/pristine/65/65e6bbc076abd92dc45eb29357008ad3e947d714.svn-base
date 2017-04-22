#include <stdio.h>
#include <stdlib.h>
#include "shared.h"
 #include <sys/mman.h>
 #include <sys/stat.h>        
 #include <fcntl.h>         


int main(int argc, char *argv[])
{
	printf("destroy.c\n");
	char* name = argv[1] ;
	int ret = shm_unlink(name);
	if(0 != ret){
		printf("destroy shm failed\n");
		return -1;
	}

	return 0;
}
