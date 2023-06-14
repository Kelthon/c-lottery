#include <omp.h>
#include <time.h>
#include <stdlib.h>

#include "metrics.h"


double metrics_speedup(double serial_time, double parallel_time) {
    return serial_time / parallel_time;
}

double metrics_efficiency(double speed_up, int num_threads) {
    return speed_up / num_threads;
}

double metrics_average(int argc, double *argv) {
    double avg = 0;

    for(int i = 0; i < argc; i++) {
        avg += argv[i];
    }

    return avg / argc;
}
