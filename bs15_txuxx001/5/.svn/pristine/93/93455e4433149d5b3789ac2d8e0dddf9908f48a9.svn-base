#!/bin/bash

# Alte Prozesse abraeumen
killall erzeug1 erzeug2 erzeug3 verbr1 verbr2 verbr3 2>/dev/null


echo "Starte Erzeuger und Verbraucher"
./erzeug1 &
./verbr1 &


read -p "Druecken Sie eine beliebige Taste, um die Prozesse zu beenden"

# Vorher gestartete Prozesse abraeumen
killall erzeug1 erzeug2 erzeug3 verbr1 verbr2 verbr3 2>/dev/null
echo "Aufgeraeumt!"
