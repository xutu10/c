#include<stdio.h>
#include<string.h>


int main(int argc, char *argv[])
{
	char* input = "20 10";

	unsigned char get[2];

	sscanf(input, "%d %d",&get[0],&get[1]);
	
	int i;
	for(i = 0; i < 2;i++)
		printf ("%d\n",get[i]);

//	unsigned char j = 0x11;

//	printf ("%d\n",j);
    return 0;
}
