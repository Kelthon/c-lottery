#include <stdio.h>
#include <stdlib.h>

#include "strnum.h"

double strnum_decimal_part(double number) {
    int number_part = (int) number;
    return (number - number_part);
}

int strnum_num_partitions(int strnum_size, int partition_size) {
    int partitions, splits, fix_factor;

    splits = (strnum_size / partition_size);

    fix_factor = (strnum_size % partition_size == 0) ? 1 : 0;

    partitions = splits - fix_factor;

    return partitions;
}

int strnum_int_length(int number, int partition_size) {
    int size = snprintf(NULL, 0, "%d", number);
    int num_partitions = strnum_num_partitions(size, partition_size);

    return size + num_partitions + 1;
}

char * strnum_int(int number, int partition_size, char separator) {
    int next_partition_distance = 4;
    int string_base_size = snprintf(NULL, 0, "%d", number);
    int size = strnum_int_length(number, partition_size);
    char string[size], aux[string_base_size + 1];

    snprintf(aux, string_base_size + 1, "%d", number);

    for(int i = size - 1, j = string_base_size; i >= 0; i--) {
        if(next_partition_distance == 0) {
            string[i] = separator;
            next_partition_distance = 3;
        } else {
            string[i] = aux[j];
            next_partition_distance--;
            j--;
        }
    }

    return string;
}

char * strnum_double(double number, int partition_size, char separator, int decimal_size, char decimal_separator) {
    int string_base_size = snprintf(NULL, 0, "%.*lf", decimal_size, number);
    int num_partitions = strnum_num_partitions(string_base_size - decimal_size, partition_size);
    int size = string_base_size + num_partitions + 1;

    double decimal_part = strnum_decimal_part(number);
    double fixed_number = (decimal_part > 0) ? number - decimal_part : number;
    int dec_part = decimal_part * (pow(10, decimal_size) * (decimal_part > 0 ? 1 : -1));

    char string[size], *aux = strnum_int(fixed_number, partition_size, separator);

    snprintf(string, size, "%s%c%d", aux, decimal_separator, dec_part);
    printf("\n%s\n", string);

    return string;
}
