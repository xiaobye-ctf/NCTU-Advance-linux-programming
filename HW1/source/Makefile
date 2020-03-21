MODULES = tcp

all: 
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} CFLAGS="$(CFLAGS)" all); \
	done

clean: 
	for dir in $(MODULES); do \
		(cd $$dir; rm *.o); \
	done

