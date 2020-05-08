CC = gcc
.PHONY: all sandbox sandbox.so clean
all: sandbox.so sandbox

sandbox:
	$(CC) main.c  -o sandbox $(CFLAGS)  
sandbox.so:
	$(CC) ./lib/sandbox.c  -shared  -fPIC  -o sandbox.so -ldl $(CFLAGS) 



clean: 
	rm sandbox sanbox.so
