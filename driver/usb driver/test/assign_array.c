#include<stdio.h>

int main(int argc, char *argv[])
{
	// doesn't work
	/* char get[5]; */

	/* get = "aabb\0"; */

	/* printf ("%s\n",get); */


	unsigned char input[3];

	input[0] =x5b;
	input[1] ='a';
	input[2] =*"\x0f";

	printf ("%d %c %c\n",input[0],input[1],input[2]);
	
    return 0;
}
