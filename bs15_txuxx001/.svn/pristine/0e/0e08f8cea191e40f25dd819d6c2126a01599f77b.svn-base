#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

void msleep(unsigned int ms)
{
	struct timespec ts;

	assert(ms > 0);
	assert(ms < 1000);

	ts.tv_sec = 0;
	ts.tv_nsec = ms * 1000 * 1000;
	nanosleep(&ts, NULL);
}

int main(int argc, char *argv[])
{
	printf("gibaus.c\n");

	printf("Schlafe fuer 300ms:\n");
	msleep(300);
	printf("... und wieder aufgewacht\n");

	return 0;
}
