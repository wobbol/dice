CC=clang

NAME=dice
DEBUG=-g

all: dice dice_POSIX dice_ISO


dice: dice.c
	$(CC) -c dice.c $(DEBUG)

dice_POSIX: dice rand_dev_urandom.c
	$(CC) -c rand_dev_urandom.c $(DEBUG)
	rm lib$(NAME)_posix.a
	ar -cvq lib$(NAME)_posix.a dice.o rand_dev_urandom.o

dice_ISO: dice rand_iso.c
	$(CC) -c rand_iso.c $(DEBUG)
	rm lib$(NAME)_iso.a
	ar -cvq lib$(NAME)_iso.a dice.o rand_iso.o
