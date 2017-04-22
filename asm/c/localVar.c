#include<stdio.h>

int mul(int a, int b){
	return a*b;
}
	

int main(int argc, char *argv[])
{
    int a = 5, b = 6;
	int c = mul(a,b);
	printf ("%d\n",c);
	
    return 0;
}
