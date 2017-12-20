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

struct dice_t {
	uintmax_t faces;
	uintmax_t num;
	uintmax_t mask;
};
void init_dice(void){
	init_random();
}
void finish_dice(void){
	remove_random();
}
char *error_str[] = {
	[E_OK] = "Success",
	[E_TOO_MANY_DICE_P] = "Too many dice_t*",
	[E_ARG_GARBAGE_END] = "Argument has extra chars",

	[E_RANGE_ARGC]  = "Number of arguments out of range",
	[E_RANGE_NUM]   = "<num> must be positive",
	[E_RANGE_FACES] = "<faces> must be greater than 1",

	[E_MISSING]       = "Missing string",
	[E_MISSING_FACES] = "<faces> missing",
	[E_MISSING_SEP]   = "\"d\" not found",

	[E_ARG_GARBAGE_BEGIN] = "<num> missing",
	[E_MISSING_NUM]       = "<num> missing",
};

char *dice_strerror(enum dice_error_e e)
{
	if(e < E_NERR)
		return error_str[e];

	char tmp[50];
	snprintf(&tmp, 50, "%lu", e);
	return tmp;
}

void dice_perror(const char *const arg)
{
	fprintf(stderr, "%s: %s.\n", arg, dice_strerror(dice_error));
}

int dice_valid(struct dice_t *dice)
{
	/* check ranges */
	if(dice->faces < 2){
		debug_puts("fail");
		dice_error = E_RANGE_FACES;
		return 1;
	}

	return 0;
}

int dice_parse_one(struct dice_t *dice, const char *const s)
{
	char *e_tmp, *tmp;
	char dice_string[MAX_ARG_SZ];

	strncpy(dice_string, s, MAX_ARG_SZ);
	tmp = &dice_string[1];

	if(!tmp){
		debug_puts("fail");
		dice_error = E_MISSING;
		goto e_dice;
	}
	if(tmp[0] == '-'){

		if(strlen(tmp) < 2){
			debug_puts("fail");
			dice_error = E_MISSING_FACES;
		}else if(!isdigit(tmp[1])){
			debug_puts("fail");
			dice_error = E_MISSING_FACES;
		}else{
			debug_puts("fail");
			dice_error = E_RANGE_FACES;
		}

		goto e_dice;
	}

	dice->faces = strtoumax(tmp, &e_tmp, 10);
	dice->num = 1;

	if(e_tmp == tmp){
		debug_puts("fail");
		dice_error = E_MISSING_FACES;
		goto e_dice;
	}
	if(tmp){
		//TODO: What are the consiquences of removeing this?
		//dice->scale = strtoumax(tmp, &e_tmp, 10);
		strtoumax(tmp, &e_tmp, 10);
	}

	if(s+strlen(s) != e_tmp || (e_tmp[0] == 'd')){
		debug_puts("fail");
		dice_error = E_ARG_GARBAGE_END;
		goto e_dice;
	}
	if(dice_valid(dice))
		goto e_dice;


	return 1;

e_dice:
	return 0;
}

/*
 * format of s:
 * AdB or dB, where A is the number of dice to roll and
 *  B is the face count of the dice.
 *
 * return value:
 *  1 when, dice at dice_off is initalized.
 *  0 when, dice at dice_off is not initalized.
 */
int dice_parse(struct dice_t **const dice, const char *const s)

{
	static struct dice_t ret;
	*dice = &ret;
	/* dX *
	 * ^  */
	if(!s)
		return 0;
	if(s[0] == 'd') {

		return dice_parse_one(*dice, s);
	}

	struct dice_t *d = *dice;

	if(!isdigit(s[0])){
		debug_puts("fail");
		dice_error = E_ARG_GARBAGE_BEGIN;
		goto e_dice;
	}

	char dice_string[MAX_ARG_SZ];
	strncpy(dice_string, s, MAX_ARG_SZ);

	char *e_tmp, *tmp;

	/* XdX or dX *
	 * ^       ^ */
	tmp = strtok(dice_string, "d");

	if(!tmp){
		debug_puts("fail");
		dice_error = E_MISSING;
		goto e_dice;
	}
	if(!strcmp(tmp, s)){
		debug_puts("fail");
		dice_error = E_MISSING_SEP;
		goto e_dice;
	}
	if(tmp[0] == '-'){
		debug_puts("fail");
		dice_error = E_RANGE_NUM;
		goto e_dice;
	}

	d->num = strtoumax(tmp, &e_tmp, 10);

	/* XdX or dX *
	 * ^       ^ */
	if(e_tmp == tmp){
		debug_puts("fail");
		dice_error = E_MISSING_NUM;
	}

	/* XdX *
	 *   ^ */
	tmp = strtok(NULL, "\0");

	if(!tmp){
		debug_puts("fail");
		dice_error = E_MISSING_FACES;
		goto e_dice;
	}
	if(tmp[0] == '-'){
		debug_puts("fail");
		dice_error = E_RANGE_FACES;
		goto e_dice;
	}

	d->faces = strtoumax(tmp, &e_tmp, 10);

	if(e_tmp == tmp){
		debug_puts("fail");
		dice_error = E_MISSING_FACES;
		goto e_dice;
	}
	if(tmp+strlen(tmp) != e_tmp){
		debug_puts("fail");
		dice_error = E_ARG_GARBAGE_END;
		goto e_dice;
	}
	if(dice_valid(d))
		goto e_dice;

	return 1;
e_dice:

	*dice = (struct dice_t *)NULL;
	return 0;
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
