CC=clang

hiera: src/hiera.h
	$(CC) -Wall -o hiera -std=c99 src/hiera-cli.c src/hiera.c src/log.c -ljansson

hiera.o: src/hiera.c src/hiera.h
	$(CC) -fpic -g -Wall -c src/hiera.c -std=c99

.PHONY: shared
shared: hiera.o
	$(CC) -shared -W1,-soname,libhiera.so.1 -o libhiera.so.1.0.1 hiera.o -ljansson

.PHONY: clean
clean:
	@rm -f *.o libhiera.so* hiera
