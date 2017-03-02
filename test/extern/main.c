#include "src.h"

int c; //= 7; // must definite here
// 2. without definition here is also ok
int main(int argc, char const *argv[])
{
	//int c; doesnot work
	c = 8; //it works, init here
	//c = assign();
	print();
	return 0;
	//gcc file.c main.c -o run
}