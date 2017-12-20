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
	[E_ARG_GARBAGE_END] = "Argument has extra chars",

	[E_RANGE_ARGC]  = "Number of arguments out of range",
	[E_RANGE_NUM]   = "<num> must be positive",
	[E_RANGE_FACES] = "<faces> must be greater than 1",

	[E_MISSING]       = "Missing string",
	[E_MISSING_FACES] = "<faces> missing",
	[E_MISSING_SEP]   = "\"d\" not found",

	[E_NOT_FOUND] = "Dice string not found",

	[E_ARG_GARBAGE_BEGIN] = "<num> missing",
	[E_MISSING_NUM]       = "<num> missing",
};

char *dice_strerror(enum dice_error_e e)
{
	if(e < E_NERR)
		return error_str[e];

	static char tmp[50];
	snprintf(tmp, 50, "%u", e);
	return tmp;
}

void dice_perror(const char *const arg)
{
	fprintf(stderr, "%s: %s.\n", arg, dice_strerror(dice_error));
}

int valid_dice(struct dice_t *dice)
{
	/* check ranges */
	if(dice->faces < 2){
		debug_puts("fail");
		dice_error = E_RANGE_FACES;
		return 0;
	}

	return 1;
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
int dice_parse(struct dice_t *const dice, const char *s)
{
	if(!s)
		return 0;

	struct dice_t *d = dice;

	while(isspace(*s))
		++s;

	if(s[0] == 'd') {
		d->num = 1;
		++s;
		goto parse_one;
	}

	if(!isdigit(s[0])){
		debug_puts("fail");
		dice_error = E_NOT_FOUND;
		return 0;
	}

	char *e_tmp;

	d->num = strtoumax(s, &e_tmp, 10);

	if(e_tmp == s){
		debug_puts("fail");
		dice_error = E_MISSING_NUM;
		return 0;
	}
	if(*e_tmp != 'd') {
		debug_puts("fail");
		dice_error = E_MISSING_SEP;
		return 0;
	}
	s = ++e_tmp;
parse_one:

	d->faces = strtoumax(s, &e_tmp, 10);

	if(e_tmp == s){
		debug_puts("fail");
		dice_error = E_MISSING_FACES;
		return 0;
	}

	if(!valid_dice(d)) {/* sets dice_error */
		debug_puts("fail");
		return 0;
	}

	return 1;
}

char *dice_str(struct dice_t *dice)
{
	static char str[MAX_ARG_SZ];
	snprintf(
	str,
	MAX_ARG_SZ,
	"%" PRIuMAX "d%" PRIuMAX,
	dice->num,
	dice->faces);
	return str;
}
void init_mask(struct dice_t *dice)
{
	uintmax_t *f = &dice->faces;
	uintmax_t *m = &dice->mask;
	uintmax_t msb = 1 + log2(*f);

	*m = 0;
	for(int i = 0; i < msb; ++i) {
		*m |= 1<<i;
	}
}

char *dice_rtd(struct dice_t *dice)
{
	static char str[MAX_ARG_SZ];
	int read;
	init_mask(dice);//TODO: Move this to the parse stuff.

	int off = 0;
	int bytes_left = MAX_ARG_SZ;
		uintmax_t r;
	while (dice->num--){
retry:
		r = random_number();
		r &= dice->mask;
		if(r > dice->faces)
			goto retry;
		if(r == 0)
			r = dice->faces;
		read = snprintf(&str[off], bytes_left, "%" PRIuMAX " ", r);

		bytes_left -= read;
		off += read;

		if(bytes_left <= 0){
			break;
		}
	}
	return str;
}
