#ifndef h_rand_gen__
#define h_rand_gen__

struct dice_t {
	uintmax_t faces;
	uintmax_t num;  /* times to roll dice */
	uintmax_t mask; /* assert((faces & mask) == faces) */
};

enum dice_error_e{
	E_OK = 0,
	E_ARG_GARBAGE_END,
	E_ARG_GARBAGE_BEGIN,

	E_RANGE_NUM,
	E_RANGE_ARGC,
	E_RANGE_FACES,

	E_NOT_FOUND,
	E_MISSING,
	E_MISSING_NUM,
	E_MISSING_SEP,
	E_MISSING_FACES,

	E_NERR,
};

extern enum dice_error_e dice_error;

char *dice_strerror(enum dice_error_e e);
void dice_perror(const char *const arg);
int dice_parse(struct dice_t *const dice, const char *const s);
char *dice_str(struct dice_t *dice);
char *dice_rtd(struct dice_t *dice);
void init_dice(void);
void finish_dice(void);

#endif
