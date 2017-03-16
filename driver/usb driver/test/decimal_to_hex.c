#include<stdio.h>
#include<string.h>


int main(int argc, char *argv[])
{
    int a = 10;
	char b[1];
	unsigned char ret[6] = "";
	unsigned char* c ="\x0b";
	unsigned char* d ="\x0";
	if(a < 16)
		sprintf(b,"%X",a);
	
	/* printf("%s\n",b); */
	/* strcat(ret,d); */
	/* strcat(ret,b); */
	/* strcat(ret,c); */
	/* printf("%s\n",ret); */

	unsigned char* input = "\x01\x11";

	printf ("%d\n",*input);

	unsigned char aa[1] = "\x02";

	printf ("%d\n",*aa);
	
    return 0;
}
