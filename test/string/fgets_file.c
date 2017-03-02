/*
 *exercise fgets read from and write to a file
 *tip: problem with eof with fgets
 *alternative fgetc, read and write by char.
 * usage of strcat, strlen
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){

	FILE *fp = NULL;
	fp = fopen("cykbd","r");
	if(!fp){
		perror("can't open the file");
		fclose(fp);
		return -1;
	}
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	printf("length of the file :%d \n",len);
	char* tmp_buffer = malloc(len*sizeof(char));
	char* buffer = malloc(len*sizeof(char));
	
	rewind(fp);

	while(fgets(tmp_buffer,len + 1,fp) != NULL){
		strcat(buffer,tmp_buffer);
		//		printf("%d\n",strlen(tmp_buffer));
	}
		
	
	printf("%s",buffer);
	fclose(fp);

	fp = fopen("des","w");
	fprintf(fp,"%s",buffer);
	fclose(fp);
	
	return 0;
}
