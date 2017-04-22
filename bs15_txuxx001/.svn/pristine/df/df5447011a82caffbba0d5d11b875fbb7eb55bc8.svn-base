#!/usr/bin/env bash

# Testprogramm fuer buchstaben.asm, Aufgabe 3.2
# Dieses Programm erwartet, dass Sie "spim" installiert haben
# und es über "spim" gestartet werden kann

echo "### Test der Aufgabe 3.2 ###"

echo "HochSchule RheinMain" | spim -file buchstaben.asm | tee buchstaben.output

echo "### Programmaufruf abgeschlossen ###"

if [ $? -ne 0 ]; then
    echo "Fehler bei Ausfuehrung des Programms" >&2
    exit 1
fi

echo "### Checkliste ###"
echo "SPIM lieferte keinen Fehler"

grep "hOCHsCHULE rHEINmAIN" buchstaben.output > /dev/null
if [ $? -ne 0 ]; then
    echo 'Fehler: Der String "hOCHsCHULE rHEINmAIN" wurde in der Ausgabe erwartet' >&2
    exit 1
else
    echo 'Der String "hOCHsCHULE rHEINmAIN" wurde in der Ausgabe gefunden (manuell nachpruefen)'
fi
