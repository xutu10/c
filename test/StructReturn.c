#include <stdio.h>

struct String{
	int count;
	char *data;
};

struct String back();

int main(int argc, char const *argv[])
{
	// 1. 初始化struct 已为其申请空间
	// 2. struct的复制为值的复制，成员指针则复制地址
	// 3. 返回值时，为值的复制，只有当返回值为地址（指针）时，若该地址在stack内则为空
	struct String from;
	printf("%d %d %d\n",&from, &from.count, &from.data );
	// 2280536 2280536 2280540 create space by initialisation
	from=back();// pointer member is copied just by address not total copied.
	//printf("%d %d %d %s\n",&from, &from.count, from.data, from.data);
	// 2280536 2280536 4206698
	getchar();

	return 0;
}

struct String back(){
	struct String a;
	a.count=5;
	char p[]="asdf\0";// no output in main method
	a.data=p;
	//a.data="qwer\0"; it's ok  2280488 2280488 4206698
	printf("%d %d %d\n",&a, &a.count, a.data );
	return a;

}

