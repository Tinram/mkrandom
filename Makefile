UNAME := $(shell uname -s)

ifeq ($(UNAME),Darwin)
	# MacOSX
	CC=clang
else
	# Unix/Linux
	CC=gcc
endif

CFLAGS=-std=c99 -O2 -Wall

all:
	$(CC) $(CFLAGS) mkrandom.c -o mkrandom

test:
	# create a random file and see if filesize matches
	./mkrandom 3m27k315 mkrandom_test.bin
	ls -l mkrandom_test.bin | grep 3173691
	rm mkrandom_test.bin

clean:
	rm mkrandom
