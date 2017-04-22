#include<iostream>
using namespace std;

int add(int a, int b){
	return a + b;	
}

int main(int argc, char *argv[])
{

	int a = 2, b = 3, c;
	c = add(a,b);
	cout<<c<<endl;
	
    return 0;
}

