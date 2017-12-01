#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "dev_random.h"

static FILE *rand_f;

reason_t strtoreason(char *const tmp, char **e_tmp)
{
	return strtoumax(tmp, e_tmp, 10);
}

void init_random(void)
{
	rand_f = fopen("/dev/random","r");
	if(!rand_f){
		perror("Cannot open /dev/random");
		exit(1);
	}
}

void remove_random(void)
{
	if(fclose(rand_f))
		perror("Cannot close /dev/random");
}

reason_t random_number(void)
{
	reason_t out;
	fread(&out, sizeof out, 1, rand_f);
	if(ferror(rand_f))
		perror("Cannot get random number");
	return out;
}
