# Makefile Uebungsaufgabe "Pipes und Signale"

CFLAGS = -std=c99 -g -W -Wall -Wno-unused-parameter -Wfatal-errors -D_GNU_SOURCE

.PHONY: all test clean distclean

all: sig_sender sig_empf1 sig_empf2 sig_empf3 erzeug1 erzeug2 erzeug3 verbr1 verbr3 init


sig_sender: sig_sender.c
	$(CC) $(CFLAGS) -o $@ $<

sig_empf1: sig_empf1.c
	$(CC) $(CFLAGS) -o $@ $<

sig_empf2: sig_empf2.c
	$(CC) $(CFLAGS) -o $@ $<

sig_empf3: sig_empf3.c
	$(CC) $(CFLAGS) -o $@ $<

erzeug1: erzeug1.c
	$(CC) $(CFLAGS) -o $@ $<

erzeug2: erzeug2.c
	$(CC) $(CFLAGS) -o $@ $<

erzeug3: erzeug3.c
	$(CC) $(CFLAGS) -o $@ $<

init: init.c
	$(CC) $(CFLAGS) -o $@ $<

verbr1: verbr1.c
	$(CC) $(CFLAGS) -o $@ $<

verbr3: verbr3.c
	$(CC) $(CFLAGS) -o $@ $<


clean:
	rm -f sig_sender sig_empf1 sig_empf2 sig_empf3 erzeug1 erzeug2 erzeug3 verbr1 verbr3 init

distclean: clean
