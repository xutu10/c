Uebungsaufgabe "Prozesse"

In dieser Praktikumsaufgabe wird das UNIX-Prozesskonzept bearbeitet.


Verzeichnisstruktur:

- vater.c                       Quellcode Eltern-Prozess
- sohn1.c                       Quellcode Kind-Prozess 1
- sohn2.c                       Quellcode Kind-Prozess 2
- beobachtungen.txt             Ihre Beobachtungen in Aufgabe 2.2 und 2.3


Sie koennen die Prozesse direkt vom Makefile aus aufrufen:

  $ make test
  cc -std=c99 -W -Wall -Wno-unused-parameter -Wfatal-errors -o vater vater.c
  cc -std=c99 -W -Wall -Wno-unused-parameter -Wfatal-errors -o sohn1 sohn1.c
  cc -std=c99 -W -Wall -Wno-unused-parameter -Wfatal-errors -o sohn2 sohn2.c
  ./vater
  Vater ...
