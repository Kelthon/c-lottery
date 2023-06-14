#include <stdio.h>
#include <stdlib.h>

#include "bet.h"

int bet_rand_int(int min, int max) {
    return (rand() % (max + 1 - min)) + min;
}

int bet_is_number_in_bet_numbers(int * bet_numbers, int interval, int number) {
    for(int i = 0; i < interval; i++) {
        if(bet_numbers[i] == number)
            return 1;
    }

    return 0;
}

int *bet_get_numbers() {
    int *bet = (int *) calloc(BET_DOZENS, sizeof(int));
    int number = 0;
    int index = 0;

    while(index < BET_DOZENS) {
        number = bet_rand_int(BET_MIN_VALUE, BET_MAX_VALUE);

        if(!bet_is_number_in_bet_numbers(bet, index, number)) {
            bet[index] = number;
            index++;
        }
    }

    return bet;
}

int bet_get_hits(int *bet_numbers, int *correct_score) {
    int hits = 0;

    for(int i = 0; i < BET_DOZENS; i++) {
        for(int j = 0; j < BET_DOZENS; j++) {
            if(bet_numbers[i] == correct_score[j]) {
                    hits++;
            }
        }
    }

    return hits;
}

char *bet_numbers_to_string(int *bet_numbers) {

    int length = snprintf(NULL, 0, "%02d", bet_numbers[0]) + 1;
    char * str = (char *) calloc(length, sizeof(char));
    snprintf(str, length, "%02d", bet_numbers[0]);

    for(int i = 1; i < BET_DOZENS; i++) {
        length = snprintf(NULL, 0, "%s %02d", str, bet_numbers[i]) + 1;
        str = (char *) realloc(str, length);
        snprintf(str, length, "%s %02d", str, bet_numbers[i]);

    }

    return str;
}
