#ifndef COMPARE_H_INCLUDED
#define COMPARE_H_INCLUDED

#define COMPARE_NUMBER_OF_TESTS 5
#define COMPARE_RESULT_ARRAY_COLUMNS 5
#define COMPARE_RESULT_ARRAY_ROWS 4

#define LINE_LEFT 0
#define LINE_RIGHT 1
#define LINE_CENTER 2
#define LINE_COLUMNS 78
#define LINE_ASCII_ROWS 8
#define LINE_SEPARATOR 45
#define LINE_CHAR_SPACE 32
#define LINE_CHAR_PERCENT 37

void compare_print_separation_line();
void compare_print_ascii_art();
int calc_middle_tabulation(const char* string);
void compare_print_line(int position, const char *string);

int **compare_set_scores();
void compare_delete_scores(int **bet_scores_buffer);
int compare_get_percentage(double value);

int serial_program(int *correct_bet);
int parallel_program(int *correct_bet, int number_threads);


#endif //COMPARE_H_INCLUDED
