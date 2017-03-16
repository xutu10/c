#include<stdio.h>

/* from void * to specific variable must be cast
   from specific variable to void* no need to be cast
 */


void output(void *text){

	char *out = (char *)text;

	printf("%s\n", out);
}

int main(){

	char *text = "hello world";
	output(text);
	int a = 0x42;
	//	output(a);  error must be ptr
	int *b = &a;
	output(b);
	
	
	return 0;
}
