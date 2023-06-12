#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "bet.h"
#include "metrics.h"
#include "strnum.h"
#include "compare.h"

int **results;
int result_counter;
int number_of_threads;
int number_of_dozens;

void compare_set_scores(int bets_num) {
   results = (int **) calloc(bets_num, sizeof(int *));

    for(int i = 0; i < bets_num; i++) {
        results[i] = bet_get_numbers(number_of_dozens, BET_MIN_VALUE, BET_MAX_VALUE);
    }
}

void compare_set_num_dozens(int num_dozens) {
    number_of_dozens = num_dozens;
}

void compare_set_num_threads(int num_threads) {
    number_of_threads = num_threads;
}

int compare_serial_lottery_attempts(int *correct_score) {
    int counter_rounds = 0;
    int round_current_hits = 0;
    int * round_guess = NULL;

    while(round_current_hits != number_of_dozens) {
        round_guess = bet_get_numbers(number_of_dozens, BET_MIN_VALUE, BET_MAX_VALUE);
        round_current_hits = bet_get_hits(round_guess, correct_score, number_of_dozens);

        counter_rounds++;

        free(round_guess);
        round_guess = NULL;
    }

    return counter_rounds;
}

int compare_parallel_lottery_attempts(int *correct_score) {
    int was_already_hinted = 0;
    int counter_rounds = 0;

    #pragma omp parallel num_threads(number_of_threads)
    {
        int round_current_hits = 0;
        int * round_guess = NULL;

        #pragma omp parallel reduction(+:counter_rounds)
        while(round_current_hits != number_of_dozens) {

            if(was_already_hinted) {

                break;
            }

            round_guess = bet_get_numbers(number_of_dozens, BET_MIN_VALUE, BET_MAX_VALUE);

            round_current_hits = bet_get_hits(round_guess, correct_score, number_of_dozens);

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

void compare_x1_test() {
    compare_serial_lottery_attempts(results[result_counter]);
}

void compare_x3_test() {
    compare_set_num_threads(3);
    compare_parallel_lottery_attempts(results[result_counter]);
}

void compare_x5_test() {
    compare_set_num_threads(5);
    compare_parallel_lottery_attempts(results[result_counter]);
}

void compare_x7_test() {
    compare_set_num_threads(7);
    compare_parallel_lottery_attempts(results[result_counter]);
}

int compare_get_percent(int value) {
    int min = 0;
    int max = COMPARE_MAXNUM_TESTS;

    return (int)((value - min) / (max - min) * 100);
}

char *compare_get_func_name(int n_threads) {
    int size;
    char *str;

    if(n_threads == 1){
        size = snprintf(NULL, 0, "%s", "Serial") + 1;
        str = (char *) calloc(size, sizeof(char));
        snprintf(str, size, "serial program");
    }

    else {
        size = snprintf(NULL, 0, "OpenMp (%d threads)", n_threads) + 1;
        str = (char *) calloc(size, sizeof(char));
        snprintf(str, size, "OpenMp (%d threads)", n_threads);
    }

    return str;
}

MetricParams **compare_get_tests(int num_tests) {
    compare_set_num_dozens(6);
    compare_set_scores(num_tests);
    double factor_percent = 100 / COMPARE_MAXNUM_TESTS;

    MetricParams **result = (MetricParams **) calloc(4, sizeof(MetricParams *));
    MetricParams *x1 = (MetricParams *) calloc(num_tests, sizeof(MetricParams));
    MetricParams *x3 = (MetricParams *) calloc(num_tests, sizeof(MetricParams));
    MetricParams *x5 = (MetricParams *) calloc(num_tests, sizeof(MetricParams));
    MetricParams *x7 = (MetricParams *) calloc(num_tests, sizeof(MetricParams));

    for(int i = 0; i < num_tests; i++) {

        x1[i].duration = metrics_get_function_duration(compare_x1_test);
        x1[i].speed_up = 1;
        x1[i].efficiency = 1;

        x3[i].duration = metrics_get_function_duration(compare_x3_test);
        x3[i].speed_up = metrics_speed_up(x1[i].duration, x3[i].duration);
        x3[i].efficiency = metrics_efficiency(x3[i].speed_up, 3);

        x5[i].duration = metrics_get_function_duration(compare_x5_test);
        x5[i].speed_up = metrics_speed_up(x1[i].duration, x5[i].duration);
        x5[i].efficiency = metrics_speed_up(x5[i].speed_up, 5);

        x7[i].duration = metrics_get_function_duration(compare_x7_test);
        x7[i].speed_up = metrics_speed_up(x1[i].duration, x7[i].duration);
        x7[i].efficiency = metrics_speed_up(x7[i].speed_up, 7);

        printf("[%3d %%] - done\n", compare_get_percent(i));
        printf("\talgorithm number of tests duration speedup efficiency\n");
        printf("\t%9s %15d %8.2lf %7.2lf %10.2lf\n", "serial", COMPARE_MAXNUM_TESTS, x1[i].duration, x1[i].speed_up, x1[i].efficiency);
        printf("\t%9s %15d %8.2lf %7.2lf %10.2lf\n", "3x thr", COMPARE_MAXNUM_TESTS, x3[i].duration, x3[i].speed_up, x3[i].efficiency);
        printf("\t%9s %15d %8.2lf %7.2lf %10.2lf\n", "5x thr", COMPARE_MAXNUM_TESTS, x5[i].duration, x5[i].speed_up, x5[i].efficiency);
        printf("\t%9s %15d %8.2lf %7.2lf %10.2lf\n", "7x thr", COMPARE_MAXNUM_TESTS, x7[i].duration, x7[i].speed_up, x7[i].efficiency);
    }

    result[0] = x1;
    result[1] = x3;
    result[2] = x5;
    result[3] = x7;

    return result;
}

double **compare_get_result_array(int num_tests) {
    MetricParams **result = compare_get_tests(num_tests), *aux;
    double **avg = (double **) calloc(COMPARE_RESULT_ARRAY_ROWS, sizeof(double *));
    double * x1_avg = (double *) calloc(COMPARE_RESULT_ARRAY_COLUMNS, sizeof(double));
    double * x3_avg = (double *) calloc(COMPARE_RESULT_ARRAY_COLUMNS, sizeof(double));
    double * x5_avg = (double *) calloc(COMPARE_RESULT_ARRAY_COLUMNS, sizeof(double));
    double * x7_avg = (double *) calloc(COMPARE_RESULT_ARRAY_COLUMNS, sizeof(double));


    aux = metrics_params_average(num_tests, result[0]);
        x1_avg[0] = (double) aux->num_threads;
        x1_avg[1] = (double) num_tests;
        x1_avg[2] = aux->duration;
        x1_avg[3] = aux->speed_up;
        x1_avg[4] = aux->efficiency;
        avg[0] = x1_avg;
        free(result[0]);
    free(aux);

    aux = metrics_params_average(num_tests, result[1]);
        x3_avg[0] = (double) aux->num_threads;
        x3_avg[1] = (double) num_tests;
        x3_avg[2] = aux->duration;
        x3_avg[3] = aux->speed_up;
        x3_avg[4] = aux->efficiency;
        avg[1] = x1_avg;
        free(result[1]);
    free(aux);

    aux = metrics_params_average(num_tests, result[2]);
        x5_avg[0] = (double) aux->num_threads;
        x5_avg[1] = (double) num_tests;
        x5_avg[2] = aux->duration;
        x5_avg[3] = aux->speed_up;
        x5_avg[4] = aux->efficiency;
        avg[2] = x1_avg;
        free(result[2]);
    free(aux);

    aux = metrics_params_average(num_tests, result[3]);
        x7_avg[0] = (double) aux->num_threads;
        x7_avg[1] = (double) num_tests;
        x7_avg[2] = aux->duration;
        x7_avg[3] = aux->speed_up;
        x7_avg[4] = aux->efficiency;
        avg[3] = x1_avg;
        free(result[3]);
    free(aux);
    aux = NULL;

    free(result);
    result = NULL;

    return avg;
}

double ** compare_get_random_result_array() {
    double **result = (double **) calloc(4, sizeof(double *));
    int num_elements = bet_rand_int(1, 9) * pow(10, bet_rand_int(3, 6));

    for(int i = 0; i < 4; i++) {
        result[i] = (double *) calloc(5, sizeof(double));
        result[i][1] = num_elements;
        result[i][0] = bet_rand_int(1, 16);

        for(int j = 2; j < 5; j++) {
            result[i][j] = bet_rand_int(1, 599) / 100.0;
        }
    }

    return result;
}

void compare_print_result_array(double **result_array) {
    for(int i = 0; i < COMPARE_RESULT_ARRAY_ROWS; i++) {

        printf("%-2d ", (int) result_array[i][0]);
        printf("%d ", (int) result_array[i][1]);

        for(int j = 2; j < COMPARE_RESULT_ARRAY_COLUMNS; j++) {
            printf("%5.2lf ", result_array[i][j]);
        }
        printf("\n");
    }
}

void compare_write_csv(const char *filename, double **result_array) {
    char *line;
    int line_size;
    FILE *fp = fopen(filename, "w+");
    char header[] = "Algorithm, Amount of elements(Tests), Time(seconds), Speedup, Efficiency\n";

    fwrite(header, sizeof(char), sizeof(header) - 1, fp);

    for(int i = 0; i < COMPARE_RESULT_ARRAY_ROWS; i++) {
        char *name = compare_get_func_name((int) result_array[i][0]);
        char *elements = strnum_int((int) result_array[i][1], STRNUM_DEFAULT_DECIMAL_PARTITION_SIZE, STRNUM_DEFAULT_PARTITION_SEPARATOR);

        line_size = snprintf(NULL, 0, "%s, %s, %.2lf, %.2lfx, %.2lf\n", name, elements, result_array[i][2], result_array[i][3], result_array[i][4]) + 1;
        line = (char *) calloc(line_size, sizeof(char));
        snprintf(line, line_size, "%s, %s, %.2lf, %.2lfx, %.2lf\n", name, elements, result_array[i][2], result_array[i][3], result_array[i][4]);;

        free(name);
        free(elements);
        free(line);
    }

    fwrite(line, sizeof(char), line_size, fp);

    fclose(fp);
    line = NULL;
}
