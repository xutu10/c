#!/usr/bin/env bash

# Testprogramm fuer pascal.asm
# Dieses Programm erwartet, dass Sie "spim" installiert haben
# und es über "spim" gestartet werden kann

echo "### Test der Aufgabe 3.4 ###"

echo -e "7\n3" | spim -file pascal.asm | tee pascal.output

echo "### Programmaufruf abgeschlossen ###"

if [ $? -ne 0 ]; then
    echo "Fehler bei Ausfuehrung des Programms" >&2
    exit 1
fi

echo "### Checkliste ###"
echo "SPIM lieferte keinen Fehler"

grep "35" pascal.output > /dev/null
if [ $? -ne 0 ]; then
    echo 'Fehler: Die Zahl "35" wurde in der Ausgabe erwartet' >&2
    exit 1
else
    echo 'Die Zahl "35" wurde in der Ausgabe gefunden (manuell nachpruefen)'
fi
