#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *copy_string(const char *str);
int compare_char(char ch1, char ch2);
int compare_string(const char *str1, const char *str2);
char *concate_string(const char *str1, const char *str2);

#endif



