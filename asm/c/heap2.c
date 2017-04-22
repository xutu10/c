#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    int a =6;
	int* pa = malloc(sizeof(int));
	*pa = a;
//	printf ("%d\n",*pa);
	free(pa);
    return 0;
}
