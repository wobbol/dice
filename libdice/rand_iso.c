#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

void remove_random(void){}

void init_random(void)
{
	srand(40);
}

uintmax_t random_number(void)
{
	uintmax_t ret = 0;
	for(int i = 0; i < (sizeof ret); i += sizeof(int))
		ret |= (uintmax_t)rand() << (i * 8);
	return ret;
}
