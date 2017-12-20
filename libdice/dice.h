#ifndef h_rand_gen__
#define h_rand_gen__

struct dice_t {
	uintmax_t faces;
	uintmax_t num;  /* times to roll dice */
	uintmax_t mask; /* assert((faces & mask) == faces) */
};

enum dice_error_e{
	E_OK = 0,

	E_RANGE_FACES,

	E_MISSING,
	E_MISSING_NUM,
	E_MISSING_SEP,
	E_MISSING_FACES,

	E_NERR,
};

extern enum dice_error_e dice_error;

char *dice_strerror(enum dice_error_e e);
void dice_perror(const char *const str);
int dice_parse(struct dice_t *const d, const char *const str);
char *dice_str(struct dice_t *d);
uintmax_t dice_rtd(struct dice_t *d);
void init_dice(void);
void finish_dice(void);

#endif
