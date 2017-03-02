#include <stdio.h>

void pop(int a[]);

int main(int argc, char const *argv[])
{
	
	int b[3]={0};
	printf("%x %x %x\n",&b[0],&b[1],&b[2] );
	
	pop(b);
	printf("%d %d %d\n",b[0],b[1],b[2] );
	getchar();
	return 0;
}

void pop(int a[]){
	printf("%x %x %x\n",&a[0],&a[1],&a[2] );
	a[0]=2;
	a[1]=4;
	a[2]=6;
}
