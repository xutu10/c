int main(int argc, char *argv[])
{
    int a = 1, b = 0;

	if(a || b)
		a = a + b;
	else if(a && b)
		a = a * b;
	
    return 0;
}
