#include <omp.h>
#include <time.h>

#include "metrics.h"

double metrics_serial_function_duration(void (* function)(int), int arg) {
    double start_time, end_time;

    start_time = omp_get_wtime();
        function(arg);
    end_time = omp_get_wtime();

    return end_time - start_time;
}

double metrics_parallel_function_duration(void (* function)(int, int), int arg_a, int arg_b) {
    double start_time, end_time;

    start_time = omp_get_wtime();
        function(arg_a, arg_b);
    end_time = omp_get_wtime();

    return end_time - start_time;
}

double metrics_speed_up(double serial_time, double parallel_time) {
    return serial_time / parallel_time;
}

double metrics_efficiency(double speed_up, int num_threads) {
    return speed_up / num_threads;
}
