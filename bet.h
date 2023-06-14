#ifndef BET_H_INCLUDED
#define BET_H_INCLUDED

#define BET_MIN_VALUE 1
#define BET_MAX_VALUE 60
#define BET_DOZENS 6

/** Returns a random integer number in a range */
int bet_rand_int(int min, int max);

/** Checks if an integer is in an array of integers */
int bet_is_number_in_bet_numbers(int * bet_numbers, int interval, int number);

/** Returns a new bet as an array of integers */
int *bet_get_numbers();

/** Returns the amount hits in a bet */
int bet_get_hits(int *bet_numbers, int *correct_score);

/** Converts a bet array to a string */
char *bet_numbers_to_string(int *bet_numbers);

#endif // BET_H_INCLUDED
