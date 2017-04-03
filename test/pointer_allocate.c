#include<stdio.h>
#include<stdlib.h>

void swap(int* p1, int* p2){

	int p;
	p = *p1;
	*p1 = *p2;
	*p2 = p;
	
}

void swap_error(int* p1, int* p2){

	
	//	int *p;
	printf("%d %d\n",*p1,*p2);

	int *p = (int*) malloc(sizeof(int));
	printf("%x\n",&p);

	*p = *p1;
	printf("%d %d %d\n",*p,*p1,*p2);
	*p1 = *p2;
	printf("%d %d %d\n",*p,*p1,*p2);
	*p2 = *p;
	printf("%d %d %d\n",*p,*p1,*p2);
	free(p);
	*p = NULL;
}

int main(){

	/* int* p; */
	/* printf("%p \n",p); */

	int p1 = 2, p2 = 3;
	printf("%d, %d \n",p1,p2);
	swap(&p1,&p2);
	printf("%d, %d \n",p1,p2);
	swap_error(&p1,&p2);
	printf("%d, %d \n",p1,p2);
	
	return 0;
}
