#include <stdio.h>

//void arr();
char *arrBack();
char *potBack();

int main(int argc, char const *argv[])
{
	char *a=arrBack();
	char *b=potBack();
	printf("%s %d %s %d\n",a,a,b,b );// zxc 4206719 null 2280474
	getchar();
	return 0;
}

void arr(){
	char *p="qwer\0";// in heap
	char q[]="asd\0";// in stack
	printf("%d %d\n",p,q );// 4206688 2280530
}

char *arrBack(){
	char *p="zxc\0";
	printf("%d\n",p );
	return p;
}

char *potBack(){
	char p[]="mnbv\0";// initialised in stack
	printf("%d\n",p  );
	return &p;
}
