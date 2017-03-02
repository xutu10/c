#include<stdio.h>

void input(char* string, int length);

int main(){

	char firstname[10], lastname[10];

	printf("firstname:");
	input(firstname,10);
	printf("lastname:");
	input(lastname,10);
	printf("firstname:%s, lastname:%s\n",firstname,lastname);

	return 0;
}

void input(char* string, int length){

	fgets(string,length,stdin);
	int i=0;
	for(i=0;i<length;i++){
		if(string[i]=='\n'){
			string[i] = '\0';
			break;
		}		
	}	
}
