/*
  exercise for scanf from keyboard
  different between fgets() in term of reading from keyboard
  scanf() will simply exit ie stop reading further once it hits the
  space. So scanf() will end up reading just John.

 */



#include<stdio.h>
#include<stdlib.h>

struct Student{
	int age;
	char name[20];   //here problem with pointer
	                 //because must allocate memory for pointer 
};

struct Animal{

	int age;
	char* name;
};

int main(){
	//	struct Student s;
	struct Animal a;
	a.name = malloc(10*sizeof(char));
	printf("input your age:");
	scanf("%d",&a.age);
	printf("input your name:");
	scanf("%9s",a.name);
	
	printf("age: %d, name: %s ",a.age,a.name);
	free(a.name);
	return 0;
}
