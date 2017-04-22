# Ein Beispiel-Makefile fuer das erste Praktikum
# Diese Zeile ist ein Kommentar.

# Die folgende Anweisung legt eine Variable "CFLAGS" an,
# hier die Standard-Argumente fuer den Compiler-Aufruf
CFLAGS = -std=c99 -W -Wall -Wno-unused-parameter -Wfatal-errors

.PHONY: all clean distclean

# Die folgende Anweisung beschreibt eine Regel:
# - die linke Seite vom Doppelpunkt beschreibt ein sogenanntes "Target"
# - die rechte Seite listet eine oder mehrere Abhaengigkeiten auf
# - die Abhaengigkeiten sind entweder Targets oder Dateien
#
# Die erste Regel wird per default ausgefuehrt:
# Es sollen alle Programme kompiliert werden.
all: mybcp myrevbcp mybappend mycp filelength


# Anweisung, wie kompiliert werden soll
# Dabei werden Variablen in $(variablenname) passend ersetzt
mybcp: src/mybcp.c
	$(CC) $(CFLAGS) -o mybcp src/mybcp.c

# wie oben, die eingebaute Variable $@ enthaelt das Target der Regel
myrevbcp: src/myrevbcp.c
	$(CC) $(CFLAGS) -o $@ src/myrevbcp.c

# wie oben, die Variable $< enthaelt die erste Abhaengigkeit der Regel
mybappend: src/mybappend.c
	$(CC) $(CFLAGS) -o $@ $<

mycp: src/mycp.c
	$(CC) $(CFLAGS) -o $@ $<

filelength: src/filelength.c
	$(CC) $(CFLAGS) -o $@ $<


# Die "Aufraeumregel": Loesche erzeugte Binaerdateien
clean:
	rm -f mybcp myrevbcp mybappend mycp filelength

distclean: clean
