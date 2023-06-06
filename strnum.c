#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    char *string = (char *) calloc(size, sizeof(char));
    char aux[string_base_size + 1];

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

    char * string = (char *) calloc(size, sizeof(char));
    char *aux = strnum_int(fixed_number, partition_size, separator);

    snprintf(string, size, "%s%c%d", aux, decimal_separator, dec_part);

    return string;
}

char * strnum_datetime(double datetime) {
    int seconds = (int) datetime;
    struct tm *dt = localtime(&seconds);
    char dt_format[] = "%02d/%02d/%02d %02d:%02d:%02.3f";
    int string_size = snprintf(NULL, 0, dt_format, dt->tm_mday, dt->tm_mon + 1, dt->tm_year + 1900, dt->tm_hour, dt->tm_min, dt->tm_sec + strnum_decimal_part(datetime));
    char *string = (char *) calloc(string_size, sizeof(char));

    snprintf(string, string_size, dt_format, dt->tm_mday, dt->tm_mon + 1, dt->tm_year + 1900, dt->tm_hour, dt->tm_min, dt->tm_sec + strnum_decimal_part(datetime));

    return string;
}

char * strnum_concat(char *string, char *concat_string, char *concat_format) {
    int result_size;
    char *result = NULL;

    if(string == NULL) {
        result_size = snprintf(NULL, 0, "%s", concat_string) + 1;
        result = (char *) calloc(result_size, sizeof(char));
        snprintf(result, result_size, "%s", concat_string);
    }

    else if(concat_string == NULL) {
        result_size = snprintf(NULL, 0, "%s", string) + 1;
        result = (char *) calloc(result_size, sizeof(char));
        snprintf(result, result_size, "%s", string);
    }

    else {
        result_size = snprintf(NULL, 0, concat_format, string, concat_string)  + 1;
        result = (char *) calloc(result_size, sizeof(char));
        snprintf(result, result_size, concat_format, string, concat_string);
    }

    return result;
}

char *strnum_time(double seconds, int type, char * format, int plural) {
    int string_size;
    char control = 's';
    char *string = NULL;


    if(type == TYPE_SECOND) {
        double dtlf = seconds / (double) type;

        string_size = snprintf(NULL, 0, format, dtlf) + 1;

        if(plural && dtlf > 1) {
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

        if(plural && dt > 1) {
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

char * strnum_elapsed_time(double elapsed_time) {

    char d_format[] = "%02d day";
    char h_format[] = "%02d hour";
    char m_format[] = "%02d minute";
    char s_format[] = "%02.3f seconds";

    double secs;
    char *string = NULL, *aux;
    int seconds = (int) elapsed_time;


    if((seconds / TYPE_DAY) > 0) {
        aux = strnum_time(seconds, TYPE_DAY, d_format, 1);
        string = strnum_concat(string, aux, "%s, %s");
        seconds %= TYPE_DAY;
        free(aux);
        aux = NULL;
    }

    if((seconds / TYPE_HOUR) > 0) {
        aux = strnum_time(seconds, TYPE_HOUR, h_format, 1);
        string = strnum_concat(string, aux, "%s, %s");
        seconds %= TYPE_HOUR;
        free(aux);
        aux = NULL;
    }

    if((seconds / TYPE_MINUTE) > 0) {
        aux = strnum_time(seconds, TYPE_MINUTE, m_format, 1);
        string = strnum_concat(string, aux, "%s, %s");
        seconds %= TYPE_MINUTE;
        secs = seconds + strnum_decimal_part(elapsed_time);
        free(aux);
        aux = NULL;
    }

    if(seconds > 0 || elapsed_time == 0) {
        aux = strnum_time(secs, TYPE_SECOND, s_format, 0);
        string = strnum_concat(string, aux, "%s, %s");
        free(aux);
        aux = NULL;
    }

    return string;
}
