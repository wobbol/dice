CC=clang

PSEUDO=rand_gen/libc_pseudo.c -DLIBC_PSEUDO
DEV_R=rand_gen/dev_random.c -DDEV_RANDOM
NAME=dice

all: libc dev_random

libc:
	$(CC) dice.c $(PSEUDO) -o $(NAME)_p
dev_random:
	$(CC) dice.c $(DEV_R) -o $(NAME)_d
