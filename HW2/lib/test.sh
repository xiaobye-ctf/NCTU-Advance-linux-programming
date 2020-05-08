#!/bin/sh
gcc sandbox.c  -shared  -fPIC  -o sandbox.so -ldl
