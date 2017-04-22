#include<iostream>
using namespace std;

int main(int argc, char *argv[])
{
	int a = 1, b = 2;
	if(a == b)
		a = 3;
	else if(a != b)
		b = 3;
	else if(a < b)
		b = 3;
	else if(a >= b)
		b = 3;

	
	
    return 0;
}
