#! /bin/bash
gcc test_case.c -o test_case
#gcc -E test_hijack.c
gcc  -shared -DDEBUG -fpic test_hijack.c  -o test.so -ldl
#gcc  -shared  -fpic test_hijack.c  -o test.so -ldl
gcc -DDEBUG test_fork_exec.c -o test
./test

