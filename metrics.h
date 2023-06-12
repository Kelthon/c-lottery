#ifndef METRICS_H_INCLUDED
#define METRICS_H_INCLUDED

#define METRICS_TYPE_NUM_THREADS 0
#define METRICS_TYPE_DURATION 1
#define METRICS_TYPE_SPEED_UP 2
#define METRICS_TYPE_EFFICIENCY 3

typedef struct {
    int num_threads;
    double duration;
    double speed_up;
    double efficiency;
} MetricParams;

/** Returns a pointer to struct MetricsParams a serial function */
MetricParams *metrics_get_params_from_serial(void (* serial_function)());

/** Returns a pointer to struct MetricsParams a function */
MetricParams *metrics_get_params(double serial_duration, int num_threads, void (* parallel_function)());

/** Returns the execution time of a function */
double metrics_get_function_duration(void (* function)());

/** Returns the speed up metrics of a parallel function */
double metrics_speed_up(double serial_time, double parallel_time);

/** Returns the efficiency metrics of a parallel function */
double metrics_efficiency(double speed_up, int num_threads);

/** Returns the average metrics of a double vector */
double metrics_average(int argc, double *argv);

/** Returns a double vector from a MetricParams vector */
double *  metrics_params_from_vector(int metrics_type, int argc, MetricParams *argv);

/** Returns the average metrics of a MetricParams vector */
MetricParams *metrics_params_average(int argc, MetricParams *argv);

#endif // METRICS_H_INCLUDED
