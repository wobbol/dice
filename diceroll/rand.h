#ifndef __common_h
#define __common_h

#include <inttypes.h>

/* defined in either dev_urandom.c or iso_c_rand.c */
void init_random(void);
void remove_random(void);
uintmax_t random_number(void);

#endif
