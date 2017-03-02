#include<stdio.h>

void output(void *text){

	char *out = (char *)text;

	printf("%s\n", out);
}

int main(){

	char *text = "hello world";
	int a = 0x42;
	int *b = &a;
	output(b);
	
	
	return 0;
}
