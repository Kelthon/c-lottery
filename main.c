#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define BET_MIN_VALUE 1
#define BET_MAX_VALUE 60

char *bet_list_to_string(int *list, int count) {

    int length = snprintf(NULL, 0, "%02d", list[0]) + 1;
    char * str = (char *) calloc(length, sizeof(char));
    snprintf(str, length, "%02d", list[0]);

    for(int i = 0; i < count; i++) {
        length = snprintf(NULL, 0, "%s %02d", str, list[i]) + 1;
        str = (char *) realloc(str, length);
        snprintf(str, length, "%s %02d", str, list[i]);
    }

    return str;

    return NULL;
}

int bet_rand_int(int min, int max) {
    return (rand() % (max + 1 - min)) + min;
}

int bet_is_in_list(int * list, int count, int number) {
    int a = 0; 
    for(int i = 0; i < count; i++) {
        a = list[i] == number;
        if(list[i] == number) return 1;
    }

    return 0;
}

int *bet_get(int dozens, int min, int max) {
    int * bet = (int *) calloc(dozens, sizeof(int));
    int number = 0;
    int index = 0;

    while(index < dozens) {
        number = bet_rand_int(min, max);

        if(!bet_is_in_list(bet, index, number)) {
            bet[index] = number;
            index++;
        }
    }

    return bet;
}

int bet_get_hits(int *guess, int *winner, int dozens) {
    int hits = 0;

    for(int i = 0; i < dozens; i++) {
        for(int j = 0; j < dozens; j++) {
            if(guess[i] == winner[j]) hits++;
        }
    }

    return hits;
}

int bet_get_award(int * list, int dozens, int hits) {
    if(hits == dozens) printf("You Win!\nHits: %d/%d\n", hits, dozens);
    else printf("You Lost!\nHits: %d/%d\n", hits, dozens);
}

int main(int argc, char **argv) {

    srand(time(NULL));

    // 179 186
    int hits = 0;
    int dozens = 6;
    int * guess = bet_get(dozens, BET_MIN_VALUE, BET_MAX_VALUE);
    int * winner = bet_get(dozens, BET_MIN_VALUE, BET_MAX_VALUE);
    char * guess_str = bet_list_to_string(guess, dozens);
    char * winner_str = bet_list_to_string(winner, dozens);

    hits = bet_get_hits(guess, winner, dozens);
    bet_get_award(guess, dozens, hits);

    printf("%s\n%s\n", guess_str, winner_str);

    free(guess);
    guess = NULL;

    free(guess_str);
    guess_str = NULL;

    free(winner);
    winner = NULL;
    
    free(winner_str);
    winner_str = NULL;

    return 0;
}
