#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

static FILE *rand_f = NULL;

void init_random(void)
{
	if(rand_f){
		return;
	}
	rand_f = fopen("/dev/urandom","r");
	if(!rand_f){
		perror("Cannot open /dev/urandom");
		exit(1);
	}
}

void remove_random(void)
{
	if(fclose(rand_f))
		perror("Cannot close /dev/urandom");
}

uintmax_t random_number(void)
{
	uintmax_t out;
	fread(&out, sizeof(uintmax_t), 1, rand_f);
	if(ferror(rand_f))
		perror("Cannot get random number");
	return out;
}
