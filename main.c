#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "bet.h"
#include "strnum.h"
#include "metrics.h"

int serial_lottery(int num_dozens) {
    int counter_rounds = 0;
    int round_current_hits = 0;
    int * round_guess = NULL;
    int * round_correct_score = NULL;

    while(round_current_hits != num_dozens) {
        round_guess = bet_get_numbers(num_dozens, BET_MIN_VALUE, BET_MAX_VALUE);
        round_correct_score = bet_get_numbers(num_dozens, BET_MIN_VALUE, BET_MAX_VALUE);

        round_current_hits = bet_get_hits(round_guess, round_correct_score, num_dozens);

        counter_rounds++;

        free(round_guess);
        round_guess = NULL;
        free(round_correct_score);
        round_correct_score = NULL;
    }

    return counter_rounds;
}

int parallel_lottery(int num_dozens, int threads) {
    int was_already_hinted = 0;
    int counter_rounds = 0;

    #pragma omp parallel num_threads(threads)
    {
        int round_current_hits = 0;
        int * round_guess = NULL;
        int * round_correct_score = NULL;
        if(was_already_hinted) printf("sim\n");

        #pragma omp parallel reduction(:counter_rounds)
        while(round_current_hits != num_dozens) {

            #pragma omp critical
            {
                if(was_already_hinted) {
                    break;
                }
            }


            round_guess = bet_get_numbers(num_dozens, BET_MIN_VALUE, BET_MAX_VALUE);
            round_correct_score = bet_get_numbers(num_dozens, BET_MIN_VALUE, BET_MAX_VALUE);

            round_current_hits = bet_get_hits(round_guess, round_correct_score, num_dozens);

            counter_rounds++;

            free(round_guess);
            round_guess = NULL;
            free(round_correct_score);
            round_correct_score = NULL;
        }

        #pragma omp critical
        {
            was_already_hinted = 1;
        }
    }
    return counter_rounds;
}

int main(int argc, char **argv) {

    srand(time(NULL));

    printf("sorting, please wait...\n");
    printf("serial program duration: %lf seconds\n", metrics_serial_function_duration(serial_lottery, 6));
    printf("parallel with %d threads program duration: %lf seconds\n", 3, metrics_parallel_function_duration(parallel_lottery, 6, 3));
    printf("parallel with %d threads program duration: %lf seconds\n", 5, metrics_parallel_function_duration(parallel_lottery, 6, 5));
    printf("parallel with %d threads program duration: %lf seconds\n", 7, metrics_parallel_function_duration(parallel_lottery, 6, 7));

    return 0;
}
