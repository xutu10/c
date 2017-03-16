#include <stdio.h>
#include <stdlib.h>

/* why must pass data as pointer ?
/* -- no need for basic data type, for struct pointer is better
/*    a pointer just holds 32 or 64 bits, much smaller than a struct.
/* reg must be pointer because shm->base is addresse ?
/* -- from ptr to variable is ok too.

/* int32_t shm_read32(struct shm *shm, uint32_t offset, uint32_t *data) { */
/* 	uint32_t *reg = (uint32_t *) (shm->base + offset); */
/* 	*data = *reg; */
/* 	return 0; */
/* } */

/* int32_t shm_write32(struct shm *shm, uint32_t offset,  uint32_t data) { */
/* 	uint32_t *reg = (uint32_t *) (shm->base + offset); */
/* 	*reg = data; */
/* 	return 0; */
/* } */


int main(){

	int a = 10;
	int *aptr = &a;
	printf("%p,%d,%p,%d\n",&a,a,aptr,*aptr);
	//	0x7ffe4229c9dc,10,0x7ffe4229c9dc,10
	int b = *aptr;     // from addresse to int
	printf("%d\n",b);
	
	int *aptr1 = malloc(sizeof(int));
	*aptr1 = a;
	printf("%p,%d,%p,%d\n",&a,a,aptr1,*aptr1);
	free(aptr1);
	//0x7fffc20b7754,10,0xa9d420,10

	int *aptr2;
	*aptr2 = a;
	printf("%p,%d,%p,%d\n",&a,a,aptr2,*aptr2);
	//	0x7fffb838257c,10,0x7fffb8382680,10
	return 0;
}
