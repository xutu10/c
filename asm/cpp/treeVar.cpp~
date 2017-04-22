#include<iostream>
using namespace std;

class Output{
public:
	Output(int a, int b,int c):value(a),count(b),a(c){
		
	}

	~Output(){
		
	}
	
	void display(){
		cout<<value<<count <<endl;
	}

public:
	int value;
	int count;
	int a;	
};


int main(int argc, char *argv[])
{
    Output o(66,88,1);
	Output oo(6,8,2);
	cout<<&oo<<" "<<&oo.value<<" "<<&oo.count<<" "<<&oo.a<<endl;
	cout<<&o<<" "<<&o.value<<" "<<&o.count<<" "<<&o.a<<endl;
	o.display();
	oo.display();
	cout<<sizeof(Output)<<endl;
	//0x7ffd1600de90 0x7ffd1600de90 0x7ffd1600de94 0x7ffd1600de98
    //0x7ffd1600de80 0x7ffd1600de80 0x7ffd1600de84 0x7ffd1600de88

    return 0;
}
