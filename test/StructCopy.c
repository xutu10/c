#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct String{
	int count;
	char *data;
};

struct String init();
void init1(struct String *to);
void copyFromStruct(struct String *to, struct String *from);
void copyFromStruct1(struct String to, struct String from);
void totalCopy(struct String *to, struct String *from);

int main(int argc, char const *argv[])
{
	/*
	//test, if the method can return a struct 
	
	struct String get=init();
	printf("%d %d %s\n", get.data,&get.data, get.data);

*/
	
	struct String *to;
	init1(to);
	printf("%s\n",to->data );
	// variable to is still null after the methode，即便为指针参数也是copy的，
	// 指向同一地址，并将地址赋值，当原variable为null，新的copy也对原variable无影响

	/*struct String *nach1,*von1;
	von1->count=7;
	von1->data="zsxdcfv\0";
	totalCopy(nach1,von1);
	if(nach1==NULL) printf("1111b\n");
	printf("%d %d %d %d %s\n",&nach1,nach1->data,von1->data, nach1->count,nach1->data);
	//2280568 53X5456 4206688*/


	/*struct String to, from;
	from.count=4;
	from.data="zxcv\0";
	copyFromStruct1(to,from);
	printf("%d %d %s\n",from.data,to.data,to.data );
	// 6701 null 复制为参数传入，对原变量无影响*/


	getchar();
	return 0;
}

void init1(struct String *to){
	if(to!=NULL) printf("2222\n");// 初始化的指针不为null
	//if(to==NULL){
		//to=malloc(sizeof(struct String));
		to->count=6;
		if(to->data==NULL) printf("11111\n");// 初始化data为null
		to->data="mnbv\0";
	
	printf("%d\n",&to );
}

struct String init(){
	struct String neu;
	neu.count=5;
	char d[4]={'a','s','d','\0'};
	printf("%s\n",d );
	neu.data=d;
	printf("%d %d\n",neu.data,&neu.data );
	struct String copy=neu;
	printf("%d %d %s %d\n",copy.data,&copy.data,copy.data,copy.count );
	return neu;
}




void copyFromStruct1(struct String to, struct String from){
	to = from;
	printf("%s %d\n",to.data,to.count );// zxcv 4
	/*to.count=from.count;
	to.data=from.data;*/
}
void totalCopy(struct String *to, struct String *from){
	if(to==NULL){
		printf("11\n");
	to= malloc(sizeof(struct String));
	to->data=NULL;
	}
	to->count=from->count;
	if(to->data) free(to->data);
	to->data=malloc(strlen(from->data)+1);
	printf("12\n");
	strcpy(to->data,from->data);
	printf("13\n");
}
