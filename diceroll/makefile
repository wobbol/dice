CC=clang

NAME=diceroll
DEBUG=-g -Wall

all: diceroll.o libdiceroll_posix.a libdiceroll_isoc.a


diceroll.o: diceroll.c
	$(CC) -c diceroll.c $(DEBUG)

libdiceroll_posix.a: diceroll.o rand_dev_urandom.c
	$(CC) -c rand_dev_urandom.c $(DEBUG)
	ar -cvr lib$(NAME)_posix.a diceroll.o rand_dev_urandom.o


libdiceroll_isoc.a: diceroll.o rand_iso.c
	$(CC) -c rand_iso.c $(DEBUG)
	ar -cvr lib$(NAME)_isoc.a diceroll.o rand_iso.o

.PHONY: clean

clean:
	rm -f diceroll.o
	rm -f rand_dev_urandom.o
	rm -f rand_iso.o
	rm -f lib$(NAME)_isoc.a
	rm -f lib$(NAME)_posix.a
