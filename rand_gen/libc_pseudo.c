#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "libc_pseudo.h"

void remove_random(void){}

void init_random(void)
{
	srand(40);
}

/* output range [0,RAND_MAX] where RAND_MAX >= 32767 */
reason_t random_number(void)
{
	return rand();
}

/*
 * An undocumented feature of atoi on my system:
 * if the input represents a number greater than the length of int
 * atoi returns -1.
 * TODO: find out if this happens on other incarnations of atoi.
 */
reason_t strtoreason(char *const tmp, char ** e_tmp)
{
	*e_tmp = tmp+strlen(tmp);
	int t = atoi(tmp);
	return t < 0 ? INT_MAX : t;
}
