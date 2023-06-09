#ifndef STRNUM_H_INCLUDED
#define STRNUM_H_INCLUDED

#define TYPE_YEAR 31560000
#define TYPE_DAY 86400
#define TYPE_HOUR 3600
#define TYPE_MINUTE 60
#define TYPE_SECOND 1
#define STRNUM_DEFAULT_PARTITION_SIZE 3
#define STRNUM_DEFAULT_PARTITION_SEPARATOR 46
#define STRNUM_DEFAULT_DECIMAL_PARTITION_SIZE 2
#define STRNUM_DEFAULT_DECIMAL_PARTITION_SEPARATOR 44

/** Returns decimal part of a double */
double strnum_decimal_part(double number);

/** Returns the number of partitions in a size */
int strnum_num_partitions(int strnum_size);

/** Returns a size of a strnum */
int strnum_int_length(int number);

/** Returns a int number notation formatted string */
char * strnum_int(int number);

/** Returns a double number notation formatted string */
char * strnum_double(double number);

/** Returns a copy of a string */
char *strnum_str_copy(const char *string);

/** Returns a concatened string of a another 2 strings */
char *strnum_str_concat(char *first_string, char *second_string, char *concat_format);

/** Returns a date and time formatted string */
char * strnum_datetime(double datetime);

/** Returns a elapsed time formatted string */
char * strnum_elapsed_time(double elapsed_time);

#endif // STRNUM_H_INCLUDED
