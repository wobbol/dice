libdiceroll - need to roll 100d61? no problem.

[!video](https://github.com/wobbol/dice/releases/download/0.1.0/example.mp4)

Take a look at example.c to see it in action.

API overview:

diceroll_init() - Start. Idempotent.
diceroll_finish() - Cleanup. Idempotent.

diceroll_parse() - Digest dice-roll string.
  return 0, dice_error is set.
  return 1, ready to roll.

diceroll_rtd() - Roll the dice.
  return > 0, a diceroll.
  return 0, dice is finished.

diceroll_perror() - Prints given string, then the dice error.
diceroll_strerror() - Return library error string.
