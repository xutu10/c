#include<stdio.h>
#include<string.h>

/*
 * not safe at strcpy, no length check, already beyond the length of destination 
 */

int main(){
	char buffer[4];
	char* src = "12345\0";

	strcpy(buffer,src);
	printf("%s",buffer);    //12345

	return 0;
}
