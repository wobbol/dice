CC=clang

NAME=dice
DEBUG=-g

all: dice.o libdice_posix.a libdice_isoc.a


dice.o: dice.c
	$(CC) -c dice.c $(DEBUG)

libdice_posix.a: dice.o rand_dev_urandom.c
	$(CC) -c rand_dev_urandom.c $(DEBUG)
	ar -cvr lib$(NAME)_posix.a dice.o rand_dev_urandom.o


libdice_isoc.a: dice.o rand_iso.c
	$(CC) -c rand_iso.c $(DEBUG)
	ar -cvr lib$(NAME)_isoc.a dice.o rand_iso.o

.PHONY: clean

clean:
	rm -f dice.o
	rm -f rand_dev_urandom.o
	rm -f rand_iso.o
	rm -f lib$(NAME)_isoc.a
	rm -f lib$(NAME)_posix.a
