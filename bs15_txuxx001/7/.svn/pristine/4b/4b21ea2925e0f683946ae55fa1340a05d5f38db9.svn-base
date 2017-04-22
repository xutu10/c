# Makefile Uebungsaufgabe "Cache-Simulator"

CFLAGS = -std=c99 -W -Wall -Wno-unused-parameter -Wfatal-errors

.PHONY: all test clean distclean

all: cachesim

cachesim: cachesim.c
	$(CC) $(CFLAGS) -o $@ $<

test: cachesim
	./cachesim test.trc 4096 4 D

run: cachesim
	./run.sh

clean:
	rm -f cachesim

distclean: clean
