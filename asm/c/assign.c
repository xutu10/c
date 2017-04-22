#include <stdio.h>

int main(int argc, char *argv[])
{
	int a = 2;
	int b = a * 2;
	int c = a + b;
//	int c = b>5;
/* cmpl $5, -8(%rbp)
   setg %al
   movzbl %al, %eax
   movl %eax, -4(%rbp)
 */
    return 0;
}
