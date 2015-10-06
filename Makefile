CC=gcc
DEPS = driver.h debug.h cache.h block.h address.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cachesimulation: driver.o cache.o block.o address.o
	gcc -lm -o driver driver.o cache.o block.o address.o -I.
