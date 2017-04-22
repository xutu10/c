#include <stdio.h>
#include <stdlib.h>


/* 
* check the mothed _access to see if a file or folder exist
* then create a newfile in this folder and delete this file 
*/
int main(int argc, char const *argv[])
{
	if((access("1",0))!=-1){

		FILE *fp;
		fp=fopen("1/newFile","w");

		if(fp!=NULL)
			printf(" has been created\n");
		else 
			printf("cannot create \n");

		if(remove("1/newFile")==0);
			printf("delete the file success \n");
	}		

	return 0;
	
}