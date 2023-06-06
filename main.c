#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#include "bet.h"
#include "strnum.h"
#include "metrics.h"

void serial_lottery(int num_dozens) {
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

    // return counter_rounds;
}

void parallel_lottery(int num_dozens, int threads) {
    int was_already_hinted = 0;
    int counter_rounds = 0;

    #pragma omp parallel num_threads(threads)
    {
        int round_current_hits = 0;
        int * round_guess = NULL;
        int * round_correct_score = NULL;

        printf("%dth accesss %s\n", omp_get_thread_num(), was_already_hinted? "winned" : "lost");

        #pragma omp parallel reduction(+:counter_rounds)
        while(round_current_hits != num_dozens) {

            if(was_already_hinted) {

                break;
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
    // return counter_rounds;
}

int main(int argc, char **argv) {
    double serial_duration, parallel_duration_3th, parallel_duration_5th, parallel_duration_7th;
    char *formated_serial_duration, *formated_parallel_duration_3th, *formated_parallel_duration_5th, *formated_parallel_duration_7th;

    srand(time(NULL));
    printf("sorting bets, please wait ...\n");
    
    serial_duration = metrics_serial_function_duration(serial_lottery, 6);
    // parallel_duration_3th = metrics_parallel_function_duration(parallel_lottery, 6, 3);
    // parallel_duration_5th = metrics_parallel_function_duration(parallel_lottery, 6, 5);
    parallel_duration_7th = metrics_parallel_function_duration(parallel_lottery, 6, 7);

    formated_serial_duration = strnum_elapsed_time(serial_duration);
    // formated_parallel_duration_3th = strnum_elapsed_time(parallel_duration_3th);
    // formated_parallel_duration_5th = strnum_elapsed_time(parallel_duration_5th);
    formated_parallel_duration_7th = strnum_elapsed_time(parallel_duration_7th);

    printf("serial program duration: %s\n", formated_serial_duration);
    // printf("%d threads program duration: %s\n", 3, formated_parallel_duration_3th);
    // printf("%d threads program duration: %s\n", 5, formated_parallel_duration_5th);
    printf("%d threads program duration: %s\n", 7, formated_parallel_duration_7th);


    free(formated_serial_duration);
    // free(formated_parallel_duration_3th);
    // free(formated_parallel_duration_5th);
    free(formated_parallel_duration_7th);

    return 0;
}
