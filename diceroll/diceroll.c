#include <ctype.h>
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include "diceroll.h"
#include "rand.h"

#define debug_puts(s) printf("line %d: " s "\n", __LINE__)
#define MAX_ARG_SZ 256

enum diceroll_error_e diceroll_error;

void diceroll_init(void)
{
	init_random();
}
void diceroll_finish(void)
{
	remove_random();
}

static char *error_str[] = {
	[E_OK] = "Success",

	[E_RANGE_FACES] = "<faces> must be greater than 1",

	[E_MISSING]       = "Dice string not found",
	[E_MISSING_NUM]   = "<num> string",
	[E_MISSING_FACES] = "<faces> missing",
	[E_MISSING_SEP]   = "\"d\" not found",
};

char *diceroll_strerror(enum diceroll_error_e e)
{
	if(e < E_NERR)
		return error_str[e];

	static char tmp[50];
	snprintf(tmp, 50, "%u", e);
	return tmp;
}

void diceroll_perror(const char *const str)
{
	fprintf(stderr, "%s: %s.\n", str, diceroll_strerror(diceroll_error));
}

static int valid_dice(struct diceroll_t *d)
{
	/* check ranges */
	if(d->faces < 2) {
		debug_puts("fail");
		diceroll_error = E_RANGE_FACES;
		return 0;
	}

	return 1;
}

static void init_mask(struct diceroll_t *d)
{
	uintmax_t msb = 1 + log2(d->faces);

	d->mask = 0;
	for(int i = 0; i < msb; ++i) {
		d->mask |= 1 << i;
	}
}

/*
 * format of s:
 * AdB or dB, where A is the number of dice to roll and
 *  B is the face count of the dice.
 *
 * return value:
 *  1 - success, use dice
 *  0 - failure, check diceroll_error
 */
int diceroll_parse(struct diceroll_t *const d, const char *s)
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

	if(!isdigit(s[0])) {
		debug_puts("fail");
		diceroll_error = E_MISSING;
		return 0;
	}

	char *end;

	d->num = strtoumax(s, &end, 10);

	if(end == s) {
		debug_puts("fail");
		diceroll_error = E_MISSING_NUM;
		return 0;
	}
	if(*end != 'd') {
		debug_puts("fail");
		diceroll_error = E_MISSING_SEP;
		return 0;
	}
	s = ++end;
parse_one:

	d->faces = strtoumax(s, &end, 10);

	if(end == s) {
		debug_puts("fail");
		diceroll_error = E_MISSING_FACES;
		return 0;
	}

	if(!valid_dice(d)) { /* sets diceroll_error */
		return 0;
	}
	init_mask(d);

	return 1;
}

char *diceroll_str(struct diceroll_t *d)
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

uintmax_t diceroll_rtd(struct diceroll_t *const d)
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
