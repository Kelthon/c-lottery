#include <omp.h>
#include <time.h>

#include "metrics.h"

double metrics_function_duration(void (* function)()) {
    double start_time, end_time;

    start_time = omp_get_wtime();
    printf("started at: %lf\n\n", start_time);

    function();

    end_time = omp_get_wtime();
    printf("\nended at: %lf\n", start_time);

    return end_time - start_time;
}

double metrics_speed_up(double serial_time, double parallel_time) {
    return serial_time / parallel_time;
}

double metrics_efficiency(double speed_up, int num_threads) {
    return speed_up / num_threads;
}
