#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <error.h>

typedef struct {
	uint32_t handle;
} NewFileResponse;

int main(int argc, char const *argv[])
{
	// test perror
	int a =1;
	if(a)
		perror("i am true");
	else
		perror("i am false");


	/*//test sprintf, dann free
	char* b = malloc(sizeof(4));
	sprintf(b,"asbb");
	printf("%s\n",b );
	free(b);
	b = NULL;
*/
	/* test, if if((a=b)) works good
	int a = 0;
	int b = 2;
	if((a=b)){
		printf("ok\n");
	}
	else
		printf("not ok\n");*/
	/*uint8_t *tmp = NULL;
	NewFileResponse *nfr = malloc(sizeof(NewFileResponse));
	nfr -> handle = 12;
	tmp = (uint8_t*) nfr;
	printf("%x %x %x %x\n",tmp[0],tmp[1],tmp[2],tmp[3] );
	
	uint32_t recv = (uint32_t)(((tmp[3]&0xFF)<<24)|((tmp[2]&0xFF)<<16)|((tmp[1]&0xFF)<<8)|(tmp[0]&0xFF));
	printf("%x\n",recv );
*/

	/*uint32_t a = 258;
	//uint8_t b = (uint8_t)(a & 0xFF);
	uint8_t b = a; //take the lower 8 bit automated
	printf(" %d %d\n",a,b );*/
	

	/*uint32_t a = 0X12345678;
	uint32_t b = htonl(a);
	printf(" %x %x\n",a,b );
	printf("%d\n",sizeof(int) );*/

	/*int a =1;
	int* p = &a;
	int** pp = &p;
	printf("P;   %p %p %d \n",&p,p,*p );
	printf("pp:   %p %p %p %d \n",&pp,pp,*pp,**pp );
*/
	/* //from uint32 to uint8
	uint32_t b = 0X12345678; 
	uint8_t c[4] = {0};
	//c = &b;
	//c = (uint8_t)b;  doesn't work
	c[0] = b;
	c[1] = b >> 8;
	c[2] = b >> 16;
	c[3] = b >> 24;
	printf("%x\n", b);
	printf("%x %x %x %x\n", c[0], c[1],c[2],c[3]);
*/
	/*//from 8 to 32 with bit handle
	printf("%d\n", sizeof(uint32_t));
	uint32_t b ;
	uint8_t c[4] = {10,10,10,1};
	printf("%x %x %x %x\n", c[0],c[1],c[2],c[3]);
	b = (uint32_t)c[0];  //doesn't work
	printf("%x\n", b);
	b =  (((c[0]) << 24)
                    | ((c[1]) << 16)
                    | ((c[2]) << 8)
                    | (c[3]));
	
	printf("%x\n", b);
	//printf("%x %x %x %x\n", c[0],c[1],c[2],c[3]);
*/

	return 0;
}