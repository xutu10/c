#include<assert.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
    int a =1;
	
	//error if false;
	// do nothing if true
	assert(a);
	printf(" a finished\n");

	int *b;  // aborted
	//	int *b = &a;
	assert(b);
	printf("b finished\n");
	
	
    return 0;
}
