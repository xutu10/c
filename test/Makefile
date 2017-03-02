# Makefile Uebungsaufgabe "Threads"

CFLAGS = -std=c99 -W -Wall -Wno-unused-parameter -Wfatal-errors

.PHONY: all test clean distclean

all:func

func:FuncAsArgu.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f func

distclean: clean
