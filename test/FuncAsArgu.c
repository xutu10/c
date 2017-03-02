#include <stdio.h>

int add(int a,int b){
	return a+b;
}

int sub(int a, int b){
  return a-b;
}

int (*fun)(int a, int b);

void show(int (*fun)(), int a, int b){
	printf("%d\n",fun(a,b) );
}

int main(int argc, char const *argv[])
{
	fun = add;
	int a = 4,b=3;
	show(fun,a,b);
	return 0;
}
