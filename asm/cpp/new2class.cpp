#include<iostream>
using namespace std;

class Output{
public:
	Output(int a, int b,int c):value(a),count(b),a(c){
		
	}

    void display(){
//		Input* i =  new Input(4); Error, class was not declared
		cout<<value<<count <<endl;
	}

public:
	int value;
	int count;
	int a;
	
};

class Input{
public:
	Input(int b):var(b){
		
	}

	 void display(){
		 // Output *op = new Output(1,2,3); it works
		cout<<var <<endl;
	}

	int var;
};


int main(int argc, char *argv[])
{
    Output* o = new Output(6,8,1);
	Output* oo = new Output(66,88,2);
	Input* i =  new Input(4);
	o -> display();
	i -> display();
	delete[] o,oo;
	delete[] i;
    return 0;
}
