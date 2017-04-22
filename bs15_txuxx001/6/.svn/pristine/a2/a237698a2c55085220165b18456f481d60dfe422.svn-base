# Makefile Uebungsaufgabe "SHM"

CFLAGS = -std=c99 -W -Wall -Wno-unused-parameter -Wfatal-errors -D_GNU_SOURCE -pthread
LIBS = -lrt -lpthread

.PHONY: all test clean distclean

all: create destroy schreiber leser philosophen

run: all
	./run.sh

create: create.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

destroy: destroy.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

schreiber: schreiber.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

leser: leser.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

philosophen: philosophen.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)


clean:
	rm -f create destroy schreiber leser philosophen

distclean: clean
