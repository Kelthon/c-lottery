#include <omp.h>
#include <time.h>
#include <stdlib.h>

#include "metrics.h"

MetricParams *metrics_get_params_from_serial(void (* serial_function)()) {

    MetricParams* params = (MetricParams *) malloc(sizeof(MetricParams));

    params->efficiency = 1;
    params->efficiency = 1;
    params->num_threads = 1;
    params->duration = metrics_get_function_duration(serial_function);

    return params;
}

MetricParams *metrics_get_params(double serial_duration, int num_threads, void (* parallel_function)()) {
    MetricParams* params = (MetricParams *) malloc(sizeof(MetricParams));

    params->num_threads = num_threads;
    params->duration = metrics_get_function_duration(parallel_function);
    params->efficiency = metrics_efficiency(serial_duration, params->duration);
    params->efficiency = metrics_efficiency(params->speed_up, params->num_threads);

    return params;
}

double metrics_get_function_duration(void (* function)()) {
    double start_time, end_time;

    start_time = omp_get_wtime();
        function();
    end_time = omp_get_wtime();

    return end_time - start_time;
}

double metrics_speed_up(double serial_time, double parallel_time) {
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

double * metrics_params_from_vector(int metrics_type, int argc, MetricParams *argv) {
    double *vector = (double *) calloc(argc, sizeof(double));
    
    
    switch (metrics_type) {
        case METRICS_TYPE_NUM_THREADS:
            for(int i = 0; i < argc; i++) {
                vector[i] = argv[i].num_threads;
            }
            break;
        
        case METRICS_TYPE_DURATION:
            for(int i = 0; i < argc; i++) {
                vector[i] = argv[i].duration;
            }
            break;

        case METRICS_TYPE_SPEED_UP:
            for(int i = 0; i < argc; i++) {
                vector[i] = argv[i].speed_up;
            }
            
            break;

        case METRICS_TYPE_EFFICIENCY:
            for(int i = 0; i < argc; i++) {
                vector[i] = argv[i].efficiency;
            }
            break;

        default:
            break;
    }

    return vector;
}

MetricParams *metrics_params_average(int argc, MetricParams *argv) {

    double *aux; 
    MetricParams *avg = (MetricParams *) malloc(sizeof(MetricParams));
    avg->num_threads = argv[0].num_threads;

    aux = metrics_params_from_vector(METRICS_TYPE_DURATION, argc, argv);
        avg->duration = metrics_average(argc, aux);
    free(aux);

    aux = metrics_params_from_vector(METRICS_TYPE_SPEED_UP, argc, argv);
        avg->speed_up = metrics_average(argc, aux);
    free(aux); 

    aux = metrics_params_from_vector(METRICS_TYPE_DURATION, argc, argv);
        avg->efficiency = metrics_average(argc, aux);
    free(aux);

    return avg;
}