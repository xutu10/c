#include<stdio.h>

int main(int argc, char *argv[])
{
	int a = 6;
	int b = 2;
	printf ("a:%d,b:%d\n",a++,++b);
	++b;
	a++;
	printf ("a:%d,b:%d\n",a,b);
	
	
    return 0;
}
