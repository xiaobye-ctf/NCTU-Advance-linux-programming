MODULES = tcp udp utils
INC = -I./include
CC = gcc

all: main.o
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} CFLAGS="$(CFLAGS)" all); \
	done
	$(CC) -o ../hw1 main.o ./tcp/*.o ./utils/*.o ./udp/*.o  $(INC) $(CFLAGS)

.c.o:
	$(CC) -c $< $(INC) $(CFLAGS)

clean: 
	for dir in $(MODULES); do \
		(cd $$dir; rm *.o); \
	done
	rm ./*.o
	rm ../hw1

