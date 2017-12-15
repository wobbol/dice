#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <inttypes.h>

#include "libdice/dice.h"

char *program_name;

void usage(int e)
{
	fprintf(stderr, "usage: %s [<num>]d<faces> ...\n", program_name);
	exit(e);
}

int main(int argc, char* argv[])
{
	program_name = argv[0];

	if (argc < 2)
		usage(1);

	init_dice();

	int error = 0;
	struct dice_t *d;

	//TODO: reimplement dicethrow
	while(*(++argv)) {
		if(dice_parse(&d, *argv)){
			printf("%s| %s\n", dice_str(d), dice_rtd(d));
		}else{
			dice_perror(*argv);
			error = 1;
		}
	}

	finish_dice();
	if(error)
		usage(1);
	return 0;
}
