#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *join1(char *a, char *b);
char *join2(char *a, char *b);
int main(int argc, char const *argv[])
{
	char a[5]="asdd";
	char b[5]="qwer";

	//char *c= join1(a,b);
	char *c = join2(a,b);
	printf("connected String:  %s\n",c );

	free(c);
	c=NULL;

	return 0;
}

char *join1(char *a, char *b){
	char *rslt=malloc((strlen(a)+strlen(b)+1)*sizeof(char));
	// check for error in malloc
	if(rslt==NULL) exit (1);
	// store the address of the created space in heap
	char *tmp=rslt;

	while(*a!='\0')
		*rslt++=*a++;
	while(*b!='\0')
		*rslt++=*b++;

	*rslt++='\0';

	return tmp;
}

char *join2(char *a, char *b){
	char *rslt=malloc((strlen(a)+strlen(b)+1)*sizeof(char));
	
	strcpy(rslt,a);
	strcat(rslt,b);

	return rslt;

}