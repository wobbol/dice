#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <inttypes.h>

#include "diceroll/diceroll.h"

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

	diceroll_init();

	int error = 0;
	struct diceroll_t d;

	uintmax_t roll;
	//TODO: reimplement dicethrow
	while(*(++argv)) {
		if(diceroll_parse(&d, *argv)){
			printf("%-7s| ", diceroll_str(&d));
			while((roll = diceroll_rtd(&d)) != 0)
				printf("%" PRIuMAX " ", roll);
			printf("\n");
		}else{
			diceroll_perror(*argv);
			error = 1;
		}
	}

	diceroll_finish();
	if(error)
		usage(1);
	return 0;
}
