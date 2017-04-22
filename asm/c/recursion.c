#include<stdio.h>

int recursion(int a){
	if(0 == a)
		return 1;
	else
		return(a*recursion(a-1));
}

int main(int argc, char *argv[])
{
	int ret = recursion(3);
	printf ("%d\n",ret);
	
    return 0;
}
