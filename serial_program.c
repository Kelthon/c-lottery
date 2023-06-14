#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "bet.h"

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
