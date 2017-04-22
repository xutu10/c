#include<iostream>
using namespace std;

class Output{
public:
	Output(int a, int b):value(a),count(b){
		
	}

	~Output(){
		
	}
	
	void display(){
		cout<<value<<count <<endl;
	}

public:
	int value;
	int count;
};


int main(int argc, char *argv[])
{
    Output o(66,88);
	Output oo(6,8);
	cout<<&oo<<" "<<&oo.value<<" "<<&oo.count<<endl;
	cout<<&o<<" "<<&o.value<<" "<<&o.count<<endl;
	//0x7ffcadda8d40 0x7ffcadda8d40 0x7ffcadda8d44
    //0x7ffcadda8d30 0x7ffcadda8d30 0x7ffcadda8d34
	o.display();
	oo.display();
	cout<<sizeof(Output)<<endl;
    return 0;
}
