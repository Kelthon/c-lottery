#include <omp.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "bet.h"
#include "strnum.h"
#include "metrics.h"
#include "compare.h"

int main(int argc, char **argv) {
    srand(time(NULL));

    char * dt;
    char line[100];
    double program_start_time, program_end_time;
    
    compare_print_ascii_art();
    compare_print_separation_line();

    dt = strnum_int(COMPARE_NUMBER_OF_TESTS);
    snprintf(line, 100, "Sorting %s Bets", dt);
    compare_print_line(LINE_CENTER, line);
    compare_print_line(LINE_CENTER, "Please Wait\n");

    free(dt);
    program_start_time = omp_get_wtime();
    dt = strnum_datetime(program_start_time);
    snprintf(line, 100, "Test start at '%s'", dt);
    compare_print_line(LINE_CENTER, line);

    int **bet_array = compare_set_scores();
    double times_avarage[4] = {0.0, 0.0, 0.0, 0.0};
    double speedup_avarage[4] = {1.0, 0.0, 0.0, 0.0};

    for(int i = 0; i < COMPARE_NUMBER_OF_TESTS; i++) {
        double start_time, end_time;

        // if(i != 0) {
        //     printf("%8s %8s %8s %8s\n","N", "threads", "time", "speedup");
        //     printf("---------------------------------------\n");
        //     for (int i = 0, j = 1; i < 4; i++, j += 2) {
        //         printf("%8d %8d %8.2lf %8.2lf\n", COMPARE_NUMBER_OF_TESTS, j, times_avarage[i], speedup_avarage[i]);
        //     }
        // }

        printf("[ %d%% ] - completed\n", compare_get_percentage(i));

        start_time = omp_get_wtime();
           serial_program(bet_array[i]);
        end_time = omp_get_wtime();
        times_avarage[0] += (end_time - start_time) / COMPARE_NUMBER_OF_TESTS; 

        start_time = omp_get_wtime();
             parallel_program(bet_array[i], 3);
        end_time = omp_get_wtime();
        times_avarage[1] += (end_time - start_time) / COMPARE_NUMBER_OF_TESTS; 
        speedup_avarage[1] += metrics_speedup(times_avarage[0], times_avarage[1]) / COMPARE_NUMBER_OF_TESTS; 


        start_time = omp_get_wtime();
           parallel_program(bet_array[i], 5);
        end_time = omp_get_wtime();
        times_avarage[2] += (end_time - start_time) / COMPARE_NUMBER_OF_TESTS;
        speedup_avarage[2] += metrics_speedup(times_avarage[0], times_avarage[2]) / COMPARE_NUMBER_OF_TESTS; 

        start_time = omp_get_wtime();
           parallel_program(bet_array[i], 7);
        end_time = omp_get_wtime();
        times_avarage[3] += (end_time - start_time) / COMPARE_NUMBER_OF_TESTS;
        speedup_avarage[3] += metrics_speedup(times_avarage[0], times_avarage[3]) / COMPARE_NUMBER_OF_TESTS;  
    }

    compare_delete_scores(bet_array);

    printf("\n\n");
    compare_print_line(LINE_CENTER, "Averages");
    compare_print_line(LINE_CENTER, "        N   threads   time  speedup    ");
    compare_print_line(LINE_CENTER, "---------------------------------------");
    for (int i = 0, j = 1; i < 4; i++, j += 2) {
        snprintf(line, 100, "%3d %9d %6.2lf %6.2lf", COMPARE_NUMBER_OF_TESTS, j, times_avarage[i], speedup_avarage[i]);
        compare_print_line(LINE_CENTER, line);
    }
    
    free(dt);
    program_end_time = omp_get_wtime();
    dt = strnum_datetime(program_end_time);
    printf("\n\n");
    snprintf(line, 100, "test ended at '%s'", dt);
    compare_print_line(LINE_CENTER, line);
    printf("\n\n");
    
    free(dt);
    dt = NULL;

    return 0;
}
