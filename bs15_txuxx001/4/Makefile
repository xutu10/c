# Makefile Uebungsaufgabe "Threads"

CFLAGS = -std=c99 -W -Wall -Wno-unused-parameter -Wfatal-errors -D_POSIX_C_SOURCE=200112L -pthread

.PHONY: all test clean distclean

all: gibaus gibaus_sem gibaus_mutex philosophen baeckerei

gibaus: gibaus.c
	$(CC) $(CFLAGS) -o $@ $<

gibaus_sem: gibaus_sem.c
	$(CC) $(CFLAGS) -o $@ $<

gibaus_mutex: gibaus_mutex.c
	$(CC) $(CFLAGS) -o $@ $<

philosophen: philosophen.c
	$(CC) $(CFLAGS) -o $@ $<

baeckerei: baeckerei.c
	$(CC) $(CFLAGS) -o $@ $<


clean:
	rm -f gibaus gibaus_sem gibaus_mutex philosophen baeckerei

distclean: clean
