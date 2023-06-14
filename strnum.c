#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "strnum.h"

double strnum_decimal_part(double number) {
    int number_part = (int) number;
    return (number - number_part);
}

int strnum_num_partitions(int strnum_size) {
    int partitions, splits, fix_factor;

    splits = (strnum_size / STRNUM_DEFAULT_PARTITION_SIZE);

    fix_factor = (strnum_size % STRNUM_DEFAULT_PARTITION_SIZE == 0) ? 1 : 0;

    partitions = splits - fix_factor;

    return partitions;
}

int strnum_int_length(int number) {
    int size = snprintf(NULL, 0, "%d", number);
    int num_partitions = strnum_num_partitions(size);

    return size + num_partitions + 1;
}

char * strnum_int(int number) {
    int next_partition_distance = 4;
    int string_base_size = snprintf(NULL, 0, "%d", number);
    int size = strnum_int_length(number);
    char *string = (char *) calloc(size, sizeof(char));
    char aux[string_base_size + 1];

    snprintf(aux, string_base_size + 1, "%d", number);

    for(int i = size - 1, j = string_base_size; i >= 0; i--) {
        if(next_partition_distance == 0) {
            string[i] = STRNUM_DEFAULT_PARTITION_SEPARATOR;
            next_partition_distance = 3;
        } else {
            string[i] = aux[j];
            next_partition_distance--;
            j--;
        }
    }

    return string;
}

char * strnum_double(double number) {
    int string_base_size = snprintf(NULL, 0, "%.*lf", STRNUM_DEFAULT_DECIMAL_PARTITION_SIZE, number);
    int num_partitions = strnum_num_partitions(string_base_size - STRNUM_DEFAULT_DECIMAL_PARTITION_SIZE);
    int size = string_base_size + num_partitions + 1;

    double decimal_part = strnum_decimal_part(number);
    double fixed_number = (decimal_part > 0) ? number - decimal_part : number;
    int dec_part = decimal_part * (pow(10, STRNUM_DEFAULT_DECIMAL_PARTITION_SIZE) * (decimal_part > 0 ? 1 : -1));

    char * string = (char *) calloc(size, sizeof(char));
    char *aux = strnum_int(fixed_number);

    snprintf(string, size, "%s%c%d", aux, STRNUM_DEFAULT_DECIMAL_PARTITION_SEPARATOR, dec_part);

    return string;
}

char * strnum_datetime(double datetime) {
    time_t seconds = (int) datetime;
    struct tm *dt = localtime(&seconds);
    char dt_format[] = "%02d/%02d/%02d %02d:%02d:%02.3f";
    int string_size = snprintf(NULL, 0, dt_format, dt->tm_mday, dt->tm_mon + 1, dt->tm_year + 1900, dt->tm_hour, dt->tm_min, dt->tm_sec + strnum_decimal_part(datetime));
    char *string = (char *) calloc(string_size, sizeof(char));

    snprintf(string, string_size, dt_format, dt->tm_mday, dt->tm_mon + 1, dt->tm_year + 1900, dt->tm_hour, dt->tm_min, dt->tm_sec + strnum_decimal_part(datetime));

    return string;
}

char *strnum_str_copy(const char *string) {
    int copy_size;
    char *copy = NULL;

    if(string != NULL) {
        copy_size = snprintf(NULL, 0, "%s", string) + 1;
        copy = (char *) calloc(copy_size, sizeof(char));
        snprintf(copy, copy_size, "%s", string);
    }

    return copy;
}

char * strnum_str_concat(char *first_string, char *second_string, char *concat_format) {
    int result_size;
    char *result = NULL;



    if(first_string == NULL) result = strnum_str_copy(second_string);

    else if(second_string == NULL) result = strnum_str_copy(first_string);

    else {

        result_size = snprintf(NULL, 0, concat_format, first_string, second_string)  + 1;
        result = (char *) calloc(result_size, sizeof(char));
        snprintf(result, result_size, concat_format, first_string, second_string);
    }

    return result;
}

char * strnum_time(double seconds, int type, char * format, int allow_plural) {
    int string_size;
    char control = 's';
    char *string = NULL;


    if(type == TYPE_SECOND) {
        double dtlf = seconds / (double) type;

        string_size = snprintf(NULL, 0, format, dtlf) + 1;

        if(allow_plural && dtlf > 1) {
            string_size++;
            string = (char *) calloc(string_size, sizeof(char));
            snprintf(string, string_size, format, dtlf);
            snprintf(string, string_size, "%s%c", string, control);
        }

        else {
            string = (char *) calloc(string_size, sizeof(char));
            snprintf(string, string_size, format, dtlf);
        }
    }

    else {
        int dt = seconds / type;

        string_size = snprintf(NULL, 0, format, dt) + 1;

        if(allow_plural && dt > 1) {
            string_size++;
            string = (char *) calloc(string_size, sizeof(char));
            snprintf(string, string_size, format, dt);
            snprintf(string, string_size, "%s%c", string, control);
        } else {
            string = (char *) calloc(string_size, sizeof(char));
            snprintf(string, string_size, format, dt);
        }
    }

    return string;
}

void aux_elapsed_time_concater(double *seconds, int type, char *string, char *format, int allow_plural) {
    char *next, *previous;
    int secs = (int) *seconds;

    if((secs / type) > 0) {
        if(type == TYPE_SECOND) next = strnum_time(*seconds, type, format, allow_plural);
        else next = strnum_time(secs, type, format, allow_plural);
        
        previous = string;

        string = strnum_str_concat(previous, next, "%s, %s");
        *seconds = secs % type;
        
        free(previous);
        previous = NULL;

        free(next);
        next = NULL;
    }
}

char * strnum_elapsed_time(double elapsed_time) {

    char d_format[] = "%02d day";
    char h_format[] = "%02d hour";
    char m_format[] = "%02d minute";
    char s_format[] = "%02.3f seconds";

    char *string = NULL;
    double secs = elapsed_time;
    
    aux_elapsed_time_concater(&secs, TYPE_DAY, string, d_format, 1);
    aux_elapsed_time_concater(&secs, TYPE_HOUR, string, h_format, 1);
    aux_elapsed_time_concater(&secs, TYPE_MINUTE, string, m_format, 1);
    aux_elapsed_time_concater(&secs, TYPE_SECOND, string, s_format, 1);

    printf("%s\n", string);

    return string;
}