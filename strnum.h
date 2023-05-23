#ifndef STRNUM_H_INCLUDED
#define STRNUM_H_INCLUDED

char * strnum_time(int number);
char * strnum_datetime(int number);

/** Returns decimal part of a double */
double strnum_decimal_part(double number);

/** Returns the number of partitions in a size */
int strnum_num_partitions(int strnum_size, int partition_size);

/** Returns a size of a strnum */
int strnum_int_length(int number, int partition_size);

/** Returns a int number notation formatted string */
char * strnum_int(int number, int partition_size, char separator);

/** Returns a double number notation formatted string */
char * strnum_double(double number, int partition_size, char separator, int decimal_size, char decimal_separator);

#endif // STRNUM_H_INCLUDED
