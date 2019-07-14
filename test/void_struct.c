#include<stdio.h>
#include<stdlib.h>

// test casting to self, void* s, (student*)s, not working
// but just because of comiling error, member name and age arenot known
// it's working, when student* s, (void*)s

struct student{
	char* name;
	int age;
};

void checkin(void* stu){
	struct student *s =(struct student*)stu;
	//printf("%x %x\n", s, stu);
	
	// stu = (struct student*)stu;
	//	char *n = "xutu";
	s->name = "xutu";
	s->age = 20;	
}

int main(){

	struct student* s;
	s=malloc(sizeof(struct student));
	printf("%x\n",s);
	checkin(s);
	printf("%s %d\n",s->name, s->age);
	(void*) s;
	printf("%x\n",s);
	free(s);

	return 0;
}

 
