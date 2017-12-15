CC=clang

NAME=test

all: iso_c dev_random

iso_c: example.c libdice/libdice_iso.a
	$(CC) example.c libdice/libdice_iso.a -o $(NAME)_p -g

dev_random: example.c libdice/libdice_posix.a
	$(CC) example.c libdice/libdice_posix.a -o $(NAME)_d -g
