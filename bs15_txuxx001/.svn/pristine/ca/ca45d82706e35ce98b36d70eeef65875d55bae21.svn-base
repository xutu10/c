#!/usr/bin/env bash

cd src

./test_3_1.sh

if [ $? -ne 0 ]; then
    echo "Test 3.1 fehlgeschlagen" >&2
	exit 1
fi

echo
echo "Test 3.1 abgeschlossen"
echo

./test_3_2.sh

if [ $? -ne 0 ]; then
    echo "Test 3.2 fehlgeschlagen" >&2
	exit 1
fi

echo
echo "Test 3.2 abgeschlossen"
echo

./test_3_4.sh

if [ $? -ne 0 ]; then
    echo "Test 3.4 fehlgeschlagen" >&2
	exit 1
fi

echo
echo "Test 3.4 abgeschlossen"
echo

