#!/bin/sh
rm sandbox.o
rm utils.o
rm sandbox.so
gcc -c sandbox.c -fpic
gcc -c utils.c -fpic
gcc sandbox.o utils.o  -shared  -fPIC  -o sandbox.so -ldl
