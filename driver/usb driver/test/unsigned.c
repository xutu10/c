#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	 
//	unsigned char* input = "\x01\x02\x03";
	unsigned char input[4];
	unsigned char* in = "\x03\x04";
	sprintf(input,"\x01\x02%s",in);

//	input[2] = 3;
//    strcpy(input[2],"\x03");
	int i,ret = 0;
	for(i = 0;i<4;i++){
		ret += input[i];
		printf ("%d : %d\n",i,input[i]);
}
	printf ("result %d\n",ret);
	
    return 0;
}
