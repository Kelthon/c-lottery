#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "bet.h"

int parallel_program(int *correct_bet, int number_threads) {
    int was_already_hinted = 0;
    int counter_rounds = 0;

    #pragma omp parallel num_threads(number_threads)
    {
        int round_current_hits = 0;
        int * round_guess = NULL;

        #pragma omp parallel reduction(+:counter_rounds)
        while(round_current_hits != BET_DOZENS) {

            if(was_already_hinted) {
                break;
            }

            round_guess = bet_get_numbers();
            round_current_hits = bet_get_hits(round_guess, correct_bet);

            counter_rounds++;

            free(round_guess);
            round_guess = NULL;
        }

        #pragma omp critical
        {
            was_already_hinted = 1;
        }
    }
    
    return counter_rounds;
}