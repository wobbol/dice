#ifndef h_rand_gen__
#define h_rand_gen__

struct diceroll_t {
	uintmax_t faces;
	uintmax_t num;  /* times to roll dice */
	uintmax_t mask; /* assert((faces & mask) == faces) */
};

enum diceroll_error_e{
	E_OK = 0,

	E_RANGE_FACES,

	E_MISSING,
	E_MISSING_NUM,
	E_MISSING_SEP,
	E_MISSING_FACES,

	E_NERR,
};

extern enum diceroll_error_e diceroll_error;

char *diceroll_strerror(enum diceroll_error_e e);
void diceroll_perror(const char *const str);
int diceroll_parse(struct diceroll_t *const d, const char *const str);
char *diceroll_str(struct diceroll_t *d);
uintmax_t diceroll_rtd(struct diceroll_t *d);
void diceroll_init(void);
void diceroll_finish(void);

#endif
