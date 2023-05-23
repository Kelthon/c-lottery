#ifndef BET_H_INCLUDED
#define BET_H_INCLUDED

#define BET_SENA 6
#define BET_QUINA 5
#define BET_QUADRA 4
#define BET_MIN_VALUE 1
#define BET_MAX_VALUE 60
#define BET_MIN_DOZENS 6
#define BET_MAX_DOZENS 20
#define BET_JACKPOT 10000000.00
#define BET_PLAYERS_NUMBER 6000000
#define BET_SENA_PRIZE_PERCENTAGE 0.62
#define BET_QUINA_PRIZE_PERCENTAGE 0.19
#define BET_QUADRA_PRIZE_PERCENTAGE 0.19
#define BET_SENA_WIN_PROBABILITY 0.000002
#define BET_QUINA_WIN_PROBABILITY 0.00000647173
#define BET_QUADRA_WIN_PROBABILITY 0.00042881646

/** Returns a random integer number in a range */
int bet_rand_int(int min, int max);

/** Checks if an integer is in an array of integers */
int bet_is_number_in_bet_numbers(int * bet_numbers, int bet_numbers_count, int number);

/** Returns a new bet as an array of integers */
int *bet_get_numbers(int dozens_amount, int min, int max);

/** Returns the amount hits in a bet */
int bet_get_hits(int *bet_numbers, int *correct_score, int dozens_amount);

/** Returns the award by hit per dozens */
double bet_get_award(int dozens_amount, int hits, int amount_winners);

/** Returns price per dozens */
double bet_get_price_per_dozens(int dozens_amount);

/** Converts a bet array to a string */
char *bet_numbers_to_string(int *bet_numbers, int bet_numbers_count);

/** Prints a bet */
void bet_print(int *bet_numbers);

/** Prints a bet result */
void bet_print_result(int *bet_numbers, int *correct_score);

#endif // BET_H_INCLUDED
