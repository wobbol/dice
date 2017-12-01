#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <inttypes.h>

#ifdef DEV_RANDOM
#include "rand_gen/dev_random.h"
#endif

#ifdef LIBC_PSEUDO
#include "rand_gen/libc_pseudo.h"
#endif

#define debug_puts(s) printf("line %d: " s "\n", __LINE__)
#define MAX_ARG_SZ 256


enum error_type{
	E_OK = 0,
	E_ARG_GARBAGE_END,
	E_ARG_GARBAGE_BEGIN,

	E_RANGE_NUM,
	E_RANGE_ARGC,
	E_RANGE_FACES,

	E_MISSING,
	E_MISSING_NUM,
	E_MISSING_SEP,
	E_MISSING_FACES,
};

struct s_dice {
	reason_t faces;
	reason_t num;
};

char *program_name;
struct s_dice *dice_list = NULL;

char *err_str(enum error_type e)
{
	switch(e){
	case E_OK:
		return "";
	case E_ARG_GARBAGE_END:
		return "argument has extra chars";

	case E_RANGE_ARGC:
		return "number of arguments out of range";
	case E_RANGE_NUM:
		return "<num> must be positive";
	case E_RANGE_FACES:
		return "<faces> must be greater than 1";

	case E_MISSING:
		return "missing string";
	case E_ARG_GARBAGE_BEGIN:
	case E_MISSING_NUM:
		return "<num> missing";
	case E_MISSING_FACES:
		return "<faces> missing";
	case E_MISSING_SEP:
		return "\"d\" not found";

	default:
		return "Write an error string!";
	}
}

void usage(int e)
{
	fprintf(stderr, "usage: %s [<num>]d<faces> ...\n", program_name);
	exit(e);
}

void print_error(const char *const arg, const enum error_type e)
{
	fprintf(stderr,"%s: %s\n", err_str(e), arg);
}

int dice_check(size_t dice_off, const char *const s)
{
	/* check ranges */
	if(dice_list[dice_off].num < 0){
		debug_puts("fail");
		print_error(s, E_RANGE_NUM);
		return 1;
	}
	if(dice_list[dice_off].faces < 2){
		debug_puts("fail");
		print_error(s, E_RANGE_FACES);
		return 1;
	}

	return 0;
}

int make_one_dice(size_t dice_off, const char *const s)
{
	char *e_tmp, *tmp;
	char dice_string[MAX_ARG_SZ];

	strncpy(dice_string, s, MAX_ARG_SZ);
	tmp = strtok(dice_string, "d");

	if(!tmp){
		debug_puts("fail");
		print_error(s, E_MISSING);
		goto e_dice;
	}
	if(!strcmp(tmp, s)){
		debug_puts("fail");
		print_error(s, E_MISSING_SEP);
		goto e_dice;
	}
	if(tmp[0] == '-'){

		if(strlen(tmp) < 2){
			debug_puts("fail");
			print_error(s, E_MISSING_FACES);
		}else if(!isdigit(tmp[1])){
			debug_puts("fail");
			print_error(s, E_MISSING_FACES);
		}else{
			debug_puts("fail");
			print_error(s, E_RANGE_FACES);
		}

		goto e_dice;
	}

	dice_list[dice_off].faces = strtoreason(tmp, &e_tmp);
	dice_list[dice_off].num = 1;

	if(e_tmp == tmp){
		debug_puts("fail");
		print_error(s, E_MISSING_FACES);
		goto e_dice;
	}
	if(tmp+strlen(tmp) != e_tmp || (e_tmp[0] == 'd')){
		debug_puts("fail");
		print_error(s, E_ARG_GARBAGE_END);
		goto e_dice;
	}
	if(dice_check(dice_off, s))
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
int make_dice(size_t dice_off, const char *const s)
{
	/* dX *
	 * ^  */
	if(s[0] == 'd')
		return make_one_dice(dice_off, s);

	struct s_dice *d = &dice_list[dice_off];

	if(!isdigit(s[0])){
		debug_puts("fail");
		print_error(s, E_ARG_GARBAGE_BEGIN);
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
		print_error(s, E_MISSING);
		goto e_dice;
	}
	if(!strcmp(tmp, s)){
		debug_puts("fail");
		print_error(s, E_MISSING_SEP);
		goto e_dice;
	}
	if(tmp[0] == '-'){
		debug_puts("fail");
		print_error(s, E_RANGE_NUM);
		goto e_dice;
	}

	d->num = strtoreason(tmp, &e_tmp);

	/* XdX or dX *
	 * ^       ^ */
	if(e_tmp == tmp){
		debug_puts("fail");
		print_error(s, E_MISSING_NUM);
	}



	/* XdX *
	 *   ^ */
	tmp = strtok(NULL, "\0");

	if(!tmp){
		debug_puts("fail");
		print_error(s, E_MISSING_FACES);
		goto e_dice;
	}
	if(tmp[0] == '-'){
		debug_puts("fail");
		print_error(s, E_RANGE_FACES);
		goto e_dice;
	}

	d->faces = strtoreason(tmp, &e_tmp);

	if(e_tmp == tmp){
		debug_puts("fail");
		print_error(s, E_MISSING_FACES);
		goto e_dice;
	}
	if(tmp+strlen(tmp) != e_tmp){
		debug_puts("fail");
		print_error(s, E_ARG_GARBAGE_END);
		goto e_dice;
	}
	if(dice_check(dice_off, s))
		goto e_dice;

	return 1;
e_dice:
	return 0;
}

void roll_the_dice(size_t dice_off)
{

	printf("%" reason_format "d%" reason_format " | ",
	dice_list[dice_off].num,
	dice_list[dice_off].faces);

	while (dice_list[dice_off].num--){
		reason_t r = random_number();
		reason_t f = dice_list[dice_off].faces;
		reason_t num = r % f;
		if(num == 0)
			num = f;
		printf("%" reason_format " ", num);
	}
	puts("");
}

int main(int argc, char* argv[])
{
	program_name = argv[0];

	if (argc < 2)
		usage(1);

	init_random();
	dice_list = malloc(sizeof *dice_list * (argc - 1));
	int error = 0;
	int num_dice = argc - 1;

	for(int i = 0; i < num_dice; ++i){
		if(make_dice(i, argv[i + 1]))
			roll_the_dice(i);
		else
			error = 1;
	}

	remove_random();
	free(dice_list);
	dice_list = NULL;

	if(error)
		usage(1);
	return 0;
}
