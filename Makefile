# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# define targets
TARGETS=kNN mk

#define object-files
OBJ=kNN.o mk.o trie.o k-d.o

build: $(TARGETS)

mk: mk.o trie.o
	$(CC) $(CFLAGS) $^ -o $@

kNN: kNN.o k-d.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

pack:
	zip -FSr 312CA_MantuMatei_Tema3.zip Makefile *.c *.h

clean:
	rm -f *.o $(TARGETS)

.PHONY: pack clean
