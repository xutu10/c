# Makefile Uebungsaufgabe "Prozesse"

CFLAGS = -std=c99 -W -Wall -Wno-unused-parameter -Wfatal-errors

.PHONY: all test clean distclean

all: vater sohn1 sohn2

vater: vater.c
	$(CC) $(CFLAGS) -o $@ $<

sohn1: sohn1.c
	$(CC) $(CFLAGS) -o $@ $<

sohn2: sohn2.c
	$(CC) $(CFLAGS) -o $@ $<


test: all
	./vater

kill:
	-killall vater sohn1 sohn2


clean:
	rm -f vater sohn1 sohn2

distclean: clean
