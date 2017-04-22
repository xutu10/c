#include<stdio.h>

int main(int argc, char *argv[])
{
    int a = 2;

	switch(a){
	case 1 : a++; break;
	case 2 : a += 2; break;
	case 3 : a += 3; break;
	}

	printf ("%d\n",a);
	
    return 0;
}
