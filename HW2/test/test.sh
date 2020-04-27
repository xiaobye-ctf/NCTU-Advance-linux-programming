#! /bin/bash
gcc -DDEBUG -shared -fpic  test_hijack.c  -o test.so
gcc -DDEBUG test_fork_exec.c -o test -ldl

