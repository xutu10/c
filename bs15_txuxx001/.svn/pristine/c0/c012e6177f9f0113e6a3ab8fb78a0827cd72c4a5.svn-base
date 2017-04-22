#!/usr/bin/env bash

# Testprogramm fuer buchstaben.asm, Aufgabe 3.1
# Dieses Programm erwartet, dass Sie "spim" installiert haben
# und es über "spim" gestartet werden kann

echo "### Test der Aufgabe 3.1 ###"

echo "HoCHSchule RhEiNmAiN UnIvErSiTy oF APPLIEd ScIENCES" | spim -file buchstaben.asm | tee buchstaben.output

echo "### Programmaufruf abgeschlossen ###"

if [ $? -ne 0 ]; then
    echo "Fehler bei Ausfuehrung des Programms" >&2
    exit 1
fi

echo "### Checkliste ###"
echo "SPIM lieferte keinen Fehler"

grep "28" buchstaben.output > /dev/null
if [ $? -ne 0 ]; then
    echo "Fehler: Die Zahl 28 wurde in der Ausgabe erwartet" >&2
    exit 1
else
    echo "Die Zahl 28 wurde in der Ausgabe gefunden (manuell nachpruefen)"
fi

grep "18" buchstaben.output > /dev/null
if [ $? -ne 0 ]; then
    echo "Fehler: Die Zahl 18 wurde in der Ausgabe erwartet" >&2
else
    echo "Die Zahl 18 wurde in der Ausgabe gefunden (manuell nachpruefen)"
fi
