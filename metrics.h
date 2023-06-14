#ifndef METRICS_H_INCLUDED
#define METRICS_H_INCLUDED

/** Returns the speedup metrics of a parallel function */
double metrics_speedup(double serial_time, double parallel_time);

/** Returns the efficiency metrics of a parallel function */
double metrics_efficiency(double speed_up, int num_threads);

/** Returns the average metrics of a double vector */
double metrics_average(int argc, double *argv);

#endif // METRICS_H_INCLUDED
