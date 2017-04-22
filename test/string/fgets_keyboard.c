#include<stdio.h>

// fgets has a problem with enter, when several lines are input

struct Student{
	char age[3];
	char name[10];
};

int main(){

	struct Student s;
	printf("input your age:");
	fgets(s.age,3,stdin);
	fflush(stdin);
	// problem with enter
	printf("input your name:");
	fgets(s.name,9,stdin);

	printf("age:%s, name:%s/n",s.age,s.name);
	return 0;
}
