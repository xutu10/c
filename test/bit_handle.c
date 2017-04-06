#include<stdio.h>

#define BIT(x) (1<<(x))

int main(){

	int base = 0x02;
	base |= BIT(2);
	printf("%x %d\n",base,base);
	base = base>>1;
	printf("%x %d\n",base,base);	

	return 0;
}
