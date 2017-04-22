#include<iostream>
using namespace std;


class Input{
public:
	Input(int b):var(b){
		
	}

	 void display(){
		 int a = 2;
		 cout<<var <<a<<endl;
	}

	int var;
};


int main(int argc, char *argv[])
{
	Input* i =  new Input(4);
	i -> var = 6;
	i -> display();
	delete[] i;
    return 0;
}
