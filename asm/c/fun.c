#include<stdio.h>

int add(int a, int b,int c, int d){

	return a + b + c + d;
	
}

void show(){
	printf ("%d\n",1111);
}

int main(int argc, char *argv[])
{

	int a = 2, b = 3, c = 1,d = 0,e;
	e = add(a,b,c,8);
	printf ("%d\n",e);
	show();
	
    return 0;
}


