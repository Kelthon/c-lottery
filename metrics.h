#ifndef METRICS_H_INCLUDED
#define METRICS_H_INCLUDED

/** Returns the execution time of a serial function */
double metrics_serial_function_duration(void (* function)(int), int arg);

/** Returns the execution time of a parallel function */
double metrics_parallel_function_duration(void (* function)(int, int), int arg_a, int arg_b);

/** Returns the speed up metrics of a parallel function */
double metrics_speed_up(double serial_time, double parallel_time);

/** Returns the efficiency metrics of a parallel function */
double metrics_efficiency(double speed_up, int num_threads);

#endif // METRICS_H_INCLUDED
