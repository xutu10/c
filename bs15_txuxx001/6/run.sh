#!/bin/sh

echo "* Aufraeumen alter Prozesse:"
killall leser schreiber

echo "* Anlegen/Loeschen des SHM:"
# Ihr Skript-Code hier:
./create myshm 500 
./schreiber myshm a 10 100 &
./schreiber myshm b 10 100 &
./leser myshm 
./destroy myshm