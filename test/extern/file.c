#include "src.h"
#include <stdio.h>

//int c = 8; // 1. it works ,init here
//int c;  //1. with extern or without is ok
// 2. without definition is also ok
//int d; another variable doesnot work
void print(){
	printf("%d\n",++c );
}
/*int assign(){
	c = 7;	
	return c;
}*/