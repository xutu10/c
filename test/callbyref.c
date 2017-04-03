#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void get_memory(char** p){

	*p = (char*) malloc(10);
	
}

char* get_value(){

   char *p = (char*)malloc(10);
   strcpy(p,"haha");

   return p;
}

int main(){

	char* p = NULL;
	get_memory(&p);
	strcpy(p,"hello world");
	printf("%s\n",p);
	free(p);
	p = NULL;

	char* value = get_value();
	printf("%s \n", value);
	free(value);
	value = NULL;
		
	return 0;
}
