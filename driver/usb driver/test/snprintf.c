#include<stdio.h>


int main(int argc, char *argv[])
{
    unsigned char* a = "\x0A";
	int b = 22;
	unsigned char ret[4];
	unsigned char tmp[3];
//	unsigned char* input = "\x16";

//	printf ("%d\n",*input);

	snprintf(tmp,3,"%X",b);
	printf ("%s\n",tmp);
	snprintf(ret,2,"%s",a);
	printf("%s\n",ret);
	snprintf(ret,3,"%s",tmp);
	
	printf("%s\n",ret);
	
    return 0;
}
