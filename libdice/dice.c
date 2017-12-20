#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include "dice.h"
#include "rand.h"

#define debug_puts(s) printf("line %d: " s "\n", __LINE__)
#define MAX_ARG_SZ 256

enum dice_error_e dice_error;

void init_dice(void){
	init_random();
}
void finish_dice(void){
	remove_random();
}
char *error_str[] = {
	[E_OK] = "Success",

	[E_RANGE_FACES] = "<faces> must be greater than 1",

	[E_MISSING]       = "Dice string not found",
	[E_MISSING_NUM]   = "<num> string",
	[E_MISSING_FACES] = "<faces> missing",
	[E_MISSING_SEP]   = "\"d\" not found",
};

char *dice_strerror(enum dice_error_e e)
{
	if(e < E_NERR)
		return error_str[e];

	static char tmp[50];
	snprintf(tmp, 50, "%u", e);
	return tmp;
}

void dice_perror(const char *const str)
{
	fprintf(stderr, "%s: %s.\n", str, dice_strerror(dice_error));
}

int valid_dice(struct dice_t *d)
{
	/* check ranges */
	if(d->faces < 2){
		debug_puts("fail");
		dice_error = E_RANGE_FACES;
		return 0;
	}

	return 1;
}

void init_mask(struct dice_t *d)
{
	uintmax_t msb = 1 + log2(d->faces);

	d->mask = 0;
	for(int i = 0; i < msb; ++i) {
		d->mask |= 1<<i;
	}
}

/*
 * format of s:
 * AdB or dB, where A is the number of dice to roll and
 *  B is the face count of the dice.
 *
 * return value:
 *  1 - success, use dice
 *  0 - failure, check dice_error
 */
int dice_parse(struct dice_t *const d, const char *s)
{
	if(!s)
		return 0;

	while(isspace(*s))
		++s;

	if(s[0] == 'd') {
		d->num = 1;
		++s;
		goto parse_one;
	}

	if(!isdigit(s[0])){
		debug_puts("fail");
		dice_error = E_MISSING;
		return 0;
	}

	char *end;

	d->num = strtoumax(s, &end, 10);

	if(end == s){
		debug_puts("fail");
		dice_error = E_MISSING_NUM;
		return 0;
	}
	if(*end != 'd') {
		debug_puts("fail");
		dice_error = E_MISSING_SEP;
		return 0;
	}
	s = ++end;
parse_one:

	d->faces = strtoumax(s, &end, 10);

	if(end == s){
		debug_puts("fail");
		dice_error = E_MISSING_FACES;
		return 0;
	}

	if(!valid_dice(d)) {/* sets dice_error */
		return 0;
	}
	init_mask(d);

	return 1;
}

char *dice_str(struct dice_t *d)
{
	static char str[MAX_ARG_SZ];
	snprintf(
	str,
	MAX_ARG_SZ,
	"%" PRIuMAX "d%" PRIuMAX,
	d->num,
	d->faces);
	return str;
}

uintmax_t dice_rtd(struct dice_t *const d)
{
	if(!d->num)
		return 0;

	--d->num;

	uintmax_t roll;
	do {
		roll = random_number();
		roll &= d->mask;
	} while(roll > d->faces);

	if(roll == 0)
		return d->faces;
	return roll;
}
