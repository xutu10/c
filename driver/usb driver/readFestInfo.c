#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fp;
	char data[1024];

	/* fp = popen("sudo fdisk -l >./info","r"); */
	/* if(fp == NULL){ */
	/* 	printf ("Failed to run command\n"); */
	/* 	exit(1); */
	/* } */

	int n = system("sudo fdisk -l >./disk");
	if (n == -1)
		printf ("Error on system execution\n");
	fp = fopen("./info.txt","r");
	fseek(fp,0,SEEK_END);
	int size = ftell(fp);
	fseek(fp,0,SEEK_SET);
	printf ("%d\n\n",size);
	size = 0;
	while( fgets(data,1023,fp) != NULL){
		printf ("%s",data);
		size++;
	}

	printf ("%d\n",size);
	
	fclose(fp);
//	printf ("%s\n",data);
	
    return 0;
}
