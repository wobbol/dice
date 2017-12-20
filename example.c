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
	struct dice_t d;

	uintmax_t roll;
	//TODO: reimplement dicethrow
	while(*(++argv)) {
		if(dice_parse(&d, *argv)){
			printf("%-7s| ", dice_str(&d));
			while((roll = dice_rtd(&d)) != 0)
				printf("%" PRIuMAX " ", roll);
			printf("\n");
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
