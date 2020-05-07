CC = gcc
.PHONY: all sandbox sandbox.so clean
all: sandbox sandbox.so

sandbox:
	$(CC) main.c -o sandbox $(CFLAGS)
sandbox.so:
	$(CC) -c ./lib/sandbox.c -fpic $(CFLAGS)
	$(CC) -c ./lib/utils.c -fpic $(CFLAGS)
	$(CC) sandbox.o utils.o  -shared  -fPIC  -o sandbox.so -ldl $(CFLAGS)



clean: 
	rm *.o 
