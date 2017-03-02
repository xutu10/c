#include<stdio.h>
#include<unistd.h>

int main(void){

	void *p;
	int a = 20;
	double b = 20.2;
	p = &a;
	printf("%d\n",*((int *)p)); // error, *p cannot output
	//type of *p is void, must be coerced to int
	p = &b;
	printf("%f\n",*((double *)p));
	return 0;
}










