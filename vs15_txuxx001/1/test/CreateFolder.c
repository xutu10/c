#include <stdio.h>

int main(int argc, char const *argv[])
{
	int status;
	/*status =mkdir("praktia3");
	(!status) ? (printf("Directory created\n")):
	(printf("unable to create directory\n"));
	system("pause");
	system("dir");*/

	status=rmdir("praktia2");
	(!status) ? (printf("Directory deleted\n")):
	(printf("unable to deleted directory\n"));
	system("dir");

	return 0;
}