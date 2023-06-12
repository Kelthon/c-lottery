#ifndef COMPARE_H_INCLUDED
#define COMPARE_H_INCLUDED

#define COMPARE_MAXNUM_TESTS 10000
#define COMPARE_RESULT_ARRAY_COLUMNS 5
#define COMPARE_RESULT_ARRAY_ROWS 4

double **compare_get_result_array(int num_tests);
double ** compare_get_random_result_array();
void compare_set_num_dozens(int num_dozens);
int compare_serial_lottery_attempts(int *correct_score);
int compare_parallel_lottery_attempts(int *correct_score);
void compare_print_result_array(double **result_array);
void compare_write_csv(const char *filename, double **result_array);

#endif //COMPARE_H_INCLUDED
