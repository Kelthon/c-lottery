#ifndef METRICS_H_INCLUDED
#define METRICS_H_INCLUDED

/** Returns the execution time of a function */
double metrics_function_duration(void (* function)());

double metrics_speed_up(double serial_time, double parallel_time);

double metrics_efficiency(double speed_up, int num_threads);

#endif // METRICS_H_INCLUDED
