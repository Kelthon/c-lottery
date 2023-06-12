#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "bet.h"
#include "strnum.h"
#include "metrics.h"
#include "compare.h"

#define LINE_LEFT 0
#define LINE_RIGHT 1
#define LINE_CENTER 2
#define LINE_COLUMNS 78
#define LINE_ASCII_ROWS 8
#define LINE_SEPARATOR 45
#define LINE_CHAR_SPACE 32
#define LINE_CHAR_PERCENT 37

void print_separation_line() {
    printf("\n");
    for(int i = 0; i < LINE_COLUMNS; i++) printf("%c", LINE_SEPARATOR);
    printf("\n\n");
}

void print_ascii_art() {
    int ascii_art_size = LINE_COLUMNS * LINE_ASCII_ROWS;
    char ascii_art[ascii_art_size+1];
    ascii_art[ascii_art_size] = 0;

    FILE *fp = fopen("ascii_art.txt", "r");

        printf("\n");
        fread(ascii_art, sizeof(char), ascii_art_size, fp);
        printf("%s",ascii_art);
        printf("\n");

    fclose(fp);
}

int calc_middle_tabulation(const char* string) {
    int tabulation;
    int size = snprintf(NULL, 0, "%s", string) + 1;
    int middle = LINE_COLUMNS / 2;

    tabulation = (int) ceil(middle + size / 2.0);

    return tabulation;
}

void print_line(int position, const char *string) {
    switch (position) {
        case LINE_CENTER:
            printf("%*s\n", calc_middle_tabulation(string), string);
            break;

        case LINE_RIGHT:
            printf("%*s\n", LINE_COLUMNS, string);
            break;

        default:
            printf("%-*s\n", LINE_COLUMNS, string);
            break;
    }
}

int count_format_args(const char* format) {
    char c = 0;
    int i = 0, argc = 0;

    do {
        c = format[i];

        if(c == LINE_CHAR_PERCENT && format[i + 1] != LINE_CHAR_PERCENT && format[i + 1] != LINE_CHAR_SPACE) argc++;

        i++;
    } while(c != 0);

    return argc;
}

/* void pl(const char* format, ...) {
 *     int argc = count_format_args(format);
 *
 *    va_list argv;
 *    va_start(argv, argc);
 *        va_arg(argv, int);
 *    va_end();
 *}
 */

void print_spaced_line(int argc, char **argv) {
    char line[LINE_COLUMNS + 1];
    int size = snprintf(NULL, 0, "%s", argv[0]) + 1;
    int line_overflow = size - LINE_COLUMNS;

    if(line_overflow > 0) {
        for(int i = 0, j = 0; i <= line_overflow; i++, j++) {
            line[j] = (j == line_overflow) ? '\0' : argv[0][i];
        }
        printf("%s\n", line);
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));

    print_ascii_art();
    print_separation_line();

    char *num_bets = strnum_int(COMPARE_MAXNUM_TESTS, STRNUM_DEFAULT_PARTITION_SIZE, STRNUM_DEFAULT_PARTITION_SEPARATOR);
    int title_size = snprintf(NULL, 0, "Sorting %s bets", num_bets) + 1;
    char title[title_size];

    snprintf(title, title_size, "Sorting %s bets", num_bets);
    print_line(LINE_CENTER, title);
    print_line(LINE_CENTER, "Please Wait");

    free(num_bets);
    num_bets = NULL;

//    double **result = compare_get_random_result_array();

    double **result = compare_get_result_array(COMPARE_MAXNUM_TESTS);
    compare_print_result_array(result);
//    compare_write_csv("c-lottery.csv", result);
    print_line(LINE_CENTER, "Done");

    for(int i = 0; i < 4; i++) free(result[i]);
    free(result);
    result = NULL;

    return 0;
}
