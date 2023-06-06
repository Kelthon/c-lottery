#include <stdio.h>
#include <stdlib.h>

#include "bet.h"

int bet_rand_int(int min, int max) {
    return (rand() % (max + 1 - min)) + min;
}

int bet_is_number_in_bet_numbers(int * bet_numbers, int bet_numbers_count, int number) {
    for(int i = 0; i < bet_numbers_count; i++) {
        if(bet_numbers[i] == number)
            return 1;
    }

    return 0;
}

int *bet_get_numbers(int dozens_amount, int min, int max) {
    int * bet = (int *) calloc(dozens_amount, sizeof(int));
    int number = 0;
    int index = 0;

    while(index < dozens_amount) {
        number = bet_rand_int(min, max);

        if(!bet_is_number_in_bet_numbers(bet, index, number)) {
            bet[index] = number;
            index++;
        }
    }

    return bet;
}

int bet_get_hits(int *bet_numbers, int *correct_score, int dozens_amount) {
    int hits = 0;

    for(int i = 0; i < dozens_amount; i++) {
        for(int j = 0; j < dozens_amount; j++) {
            if(bet_numbers[i] == correct_score[j]) {
                    hits++;
            }
        }
    }

    return hits;
}

double bet_get_award(int dozens_amount, int hits, int amount_winners) {

    double jackpot = 0;

    if(dozens_amount < BET_MIN_DOZENS || dozens_amount > BET_MAX_DOZENS) return jackpot;

    switch(hits) {
        case BET_SENA:
            jackpot = BET_JACKPOT * BET_SENA_PRIZE_PERCENTAGE / amount_winners;
            break;

        case BET_QUINA:
            jackpot = BET_JACKPOT * BET_QUINA_PRIZE_PERCENTAGE / amount_winners;
            break;

        case BET_QUADRA:
            jackpot = BET_JACKPOT * BET_QUADRA_PRIZE_PERCENTAGE / amount_winners;
            break;

        default:
            break;
    }

    return jackpot;
}

double bet_get_price_per_dozens(int dozens_amount) {
    double price = 0;

    float BET_PRICES[] = {
        5.0, 35.0, 140.0, 420.0, 1050.0,
        2310.0, 4620.0, 8580.0, 15015.50,
        25025.0, 40040.00, 61880.00,
        92820.00, 135660.00, 193800.00
    };

    if(dozens_amount < BET_MIN_DOZENS || dozens_amount > BET_MAX_DOZENS)
        return price;

    return BET_PRICES[dozens_amount - BET_MIN_DOZENS];
}

char *bet_numbers_to_string(int *bet_numbers, int bet_numbers_count) {

    int length = snprintf(NULL, 0, "%02d", bet_numbers[0]) + 1;
    char * str = (char *) calloc(length, sizeof(char));
    snprintf(str, length, "%02d", bet_numbers[0]);

    for(int i = 1; i < bet_numbers_count; i++) {
        length = snprintf(NULL, 0, "%s %02d", str, bet_numbers[i]) + 1;
        str = (char *) realloc(str, length);
        snprintf(str, length, "%s %02d", str, bet_numbers[i]);

    }

    return str;
}
