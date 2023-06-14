#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#include "bet.h"
#include "metrics.h"
#include "strnum.h"
#include "compare.h"
void compare_print_separation_line() {
    printf("\n");
    for(int i = 0; i < LINE_COLUMNS; i++) printf("%c", LINE_SEPARATOR);
    printf("\n\n");
}

void compare_print_ascii_art() {
    int ascii_art_size = LINE_COLUMNS * LINE_ASCII_ROWS;
    char ascii_art[ascii_art_size+1];
    ascii_art[ascii_art_size] = 0;

    FILE *fp = fopen("ascii_art.txt", "r");

        printf("\n");
        fread(ascii_art, sizeof(char), ascii_art_size, fp);
        printf("%s",ascii_art);
        printf("\n");

    fclose(fp);
}

int calc_middle_tabulation(const char* string) {
    int tabulation;
    int size = snprintf(NULL, 0, "%s", string) + 1;
    int middle = LINE_COLUMNS / 2;

    tabulation = (int) ceil(middle + size / 2.0);

    return tabulation;
}

void compare_print_line(int position, const char *string) {
    switch (position) {
        case LINE_CENTER:
            printf("%*s\n", calc_middle_tabulation(string), string);
            break;

        case LINE_RIGHT:
            printf("%*s\n", LINE_COLUMNS, string);
            break;

        default:
            printf("%-*s\n", LINE_COLUMNS, string);
            break;
    }
}

int **compare_set_scores() {
   int **bet_array = (int **) calloc(COMPARE_NUMBER_OF_TESTS, sizeof(int *));

    for(int i = 0; i < COMPARE_NUMBER_OF_TESTS; i++) {
        bet_array[i] = bet_get_numbers();
    }

    return bet_array;
}

void compare_delete_scores(int **bet_scores_buffer) {
    for(int i = 0; i < COMPARE_NUMBER_OF_TESTS; i++) {
        free(bet_scores_buffer[i]);
        bet_scores_buffer[i] = NULL;
    }

    free(bet_scores_buffer);
    bet_scores_buffer = NULL;
}

int compare_get_percentage(double value) {
    int percentage;
    double min, max, normal;
    
    min = 0;
    max = COMPARE_NUMBER_OF_TESTS;
    normal = (value - min) / (max - min);
    
    percentage = (int) (normal * 100.0);

    return percentage;
}

int serial_program(int *correct_bet) {
   int counter_rounds = 0;
   int round_current_hits = 0;
   int * round_guess = NULL;

   while(round_current_hits != BET_DOZENS) {
       round_guess = bet_get_numbers();
       round_current_hits = bet_get_hits(round_guess, correct_bet);

       counter_rounds++;

       free(round_guess);
       round_guess = NULL;
   }

   return counter_rounds;
}

int parallel_program(int *correct_bet, int number_threads) {
    int was_already_hinted = 0;
    int counter_rounds = 0;

    #pragma omp parallel num_threads(number_threads)
    {
        srand(time(NULL) + omp_get_thread_num());
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
