#!/bin/sh
# Aufgabe 7.2 -- Bestimmung der Hit-Rate

echo "-------------------------------------"

echo "*** Testlauf tomcatv 32K,A1,U ***"
./cachesim tomcatv.trc 32768 1 U
echo ""

echo "*** Testlauf tomcatv 32K,A2,U ***"
./cachesim tomcatv.trc 32768 2 U
echo ""

echo "*** Testlauf tomcatv 32K,A4,U ***"
./cachesim tomcatv.trc 32768 4 U
echo ""

echo "*** Testlauf tomcatv 32K,A8,U ***"
./cachesim tomcatv.trc 32768 8 U
echo ""

echo "*** Testlauf tomcatv 8K,A1,I ***"
./cachesim tomcatv.trc 8192 1 I
echo ""

echo "*** Testlauf tomcatv 8K,A2,I ***"
./cachesim tomcatv.trc 8192 2 I
echo ""

echo "*** Testlauf tomcatv 8K,A1,D ***"
./cachesim tomcatv.trc 8192 1 D
echo ""

echo "*** Testlauf tomcatv 8K,A2,D ***"
./cachesim tomcatv.trc 8192 2 D
echo ""

echo "-------------------------------------"

echo "*** Testlauf gcc 32K,A1,U ***"
./cachesim gcc.trc 32768 1 U
echo ""

echo "*** Testlauf gcc 32K,A2,U ***"
./cachesim gcc.trc 32768 2 U
echo ""

echo "*** Testlauf gcc 32K,A4,U ***"
./cachesim gcc.trc 32768 4 U
echo ""

echo "*** Testlauf gcc 32K,A8,U ***"
./cachesim gcc.trc 32768 8 U
echo ""

echo "*** Testlauf gcc 8K,A1,I ***"
./cachesim gcc.trc 8192 1 I
echo ""

echo "*** Testlauf gcc 8K,A2,I ***"
./cachesim gcc.trc 8192 2 I
echo ""

echo "*** Testlauf gcc 8K,A1,D ***"
./cachesim gcc.trc 8192 1 D
echo ""

echo "*** Testlauf gcc 8K,A2,D ***"
./cachesim gcc.trc 8192 2 D
echo ""

echo "-------------------------------------"
