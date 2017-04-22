#include<iostream>
using namespace std;

class Empty{
public:
	Empty* operator&(){
		return this;		
	}
	
};

int main(int argc, char *argv[])
{
    Empty e;
	cout<<&e<<endl;
	Empty* p = &e; // e.operator()
	cout<<p<<endl;

	cout<<sizeof(e)<<endl;
	
    return 0;
}
