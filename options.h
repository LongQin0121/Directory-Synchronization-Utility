#ifndef OPTIONS_H
#define OPTIONS_H
#include <regex.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    int a_flag;
    int i_pattern_count;
    int n_flag;
    int o_pattern_count;
    int p_flag;
    int r_flag;
    int v_flag;
    int directory_count;
    regex_t **i_pattern_list;
    regex_t **o_pattern_list;
    char **directory_list;

} options_t;

options_t *allocate_options(int i_pattern_count, int o_pattern_count, int directory_count);
void free_options(options_t *options);
char *glob2regex(const char *glob);
regex_t *compile_pattern(const char *str);
int can_sync_in_i_or_o_pattern(const char *filename, options_t *options);

#endif






