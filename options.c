#include "options.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//Allocate and initialize memory for an options_t structure.
options_t *allocate_options(int i_pattern_count, int o_pattern_count, int directory_count)
{
    int loop = 0;
    options_t *options = NULL;

    options = (options_t *)malloc(sizeof(options_t));
    if (options == NULL) {
        perror("malloc failed!");
    }

    memset(options, 0, sizeof(options_t));
    options->i_pattern_count = i_pattern_count;
    options->o_pattern_count = o_pattern_count;
    options->directory_count = directory_count;

    // Allocate memory for ignore patterns
    if (options->i_pattern_count > 0)
    {
        options->i_pattern_list = (regex_t **)malloc(sizeof(regex_t *) * options->i_pattern_count);
        
        if (options->i_pattern_list == NULL) {
        perror("Failed to allocate memory for options->i_pattern_list");
        exit(EXIT_FAILURE);
        }
        
        for (loop = 0; loop < options->i_pattern_count; loop++)
        {
            options->i_pattern_list[loop] = NULL;
        }
    }
    // Allocate memory for only patterns
    if (options->o_pattern_count > 0)
    {
        options->o_pattern_list = (regex_t **)malloc(sizeof(regex_t *) * options->o_pattern_count);
        
        if (options->o_pattern_list == NULL) {
        perror("Failed to allocate memory for options->o_pattern_list");
        exit(EXIT_FAILURE);
        }
        
        for (loop = 0; loop < options->o_pattern_count; loop++)
        {
            options->o_pattern_list[loop] = NULL;
        }
    }
    // Allocate memory for directories
    if (options->directory_count > 0)
    {
        options->directory_list = (char **)malloc(sizeof(char *) * options->directory_count);
        
        if (options->directory_list == NULL) {
        perror("Failed to allocate memory for options->directory_list");
        exit(EXIT_FAILURE);
        }

        for (loop = 0; loop < options->directory_count; loop++)
        {
            options->directory_list[loop] = NULL;
        }
    }

    return options;
}

/**
 * Free the memory allocated for the options_t structure and its members.
 * @param options A pointer to the options_t structure to be freed.
 */
void free_options(options_t *options)
{
    int loop = 0;

    if (NULL != options)
    {   
        // Free memory allocated for ignore patterns
        if (NULL != options->i_pattern_list)
        {
            for (loop = 0; loop < options->i_pattern_count; loop++)
            {
                if (NULL != options->i_pattern_list[loop])
                {
                // Free the compiled regular expression and the memory for the regex_t pointer
                    regfree(options->i_pattern_list[loop]);
                    free(options->i_pattern_list[loop]);
                }
            }
            free(options->i_pattern_list);
        }

        // Free memory allocated for O patterns
        if (NULL != options->o_pattern_list)
        {
            for (loop = 0; loop < options->o_pattern_count; loop++)
            {
                if (NULL != options->o_pattern_list[loop])
                {
                    regfree(options->o_pattern_list[loop]);
                    free(options->o_pattern_list[loop]);
                }
            }
            free(options->o_pattern_list);
        }

        // Free memory allocated for directory list
        if (NULL != options->directory_list)
        {
            for (loop = 0; loop < options->directory_count; loop++)
            {
                if (NULL != options->directory_list[loop])
                {
                    free(options->directory_list[loop]);
                }
            }
            free(options->directory_list);
        }
        free(options);
    }
}

//Provided. Convert a glob-style pattern into a regex pattern.
char *glob2regex(const char *glob)
{
    char *re = NULL;

    if (glob != NULL) {
        re = calloc(strlen(glob) * 2 + 4, sizeof(char));
        if (re == NULL) {
            return NULL;
        }
        
        char *r = re;

        *r++ = '^';
        while (*glob != '\0')
            switch (*glob) {
            case '.':
            case '\\':
            case '$': *r++ = '\\'; *r++ = *glob++;	break;
            case '*': *r++ = '.';  *r++ = *glob++;	break;
            case '?': *r++ = '.'; glob++;		break;
            case '/': free(re);
                re = NULL;
                break;
            default: *r++ = *glob++;
                break;
            }
        if (re) {
            *r++ = '$';
            *r = '\0';
        }
    }
    return re;
}

//Compiles a glob pattern into a regex_t structure for further use
regex_t *compile_pattern(const char *str)
{
    char *regex_string = NULL;
    regex_t *pattern = NULL;

    regex_string = glob2regex(str);
    pattern = (regex_t *)malloc(sizeof(regex_t));

    memset(pattern, 0, sizeof(regex_t));

    // Compile the regex; if failed, clean up and return NULL
    if (regcomp(pattern, regex_string, REG_EXTENDED))
    {
        free(regex_string);
        free(pattern);
        return NULL;
    }
    free(regex_string);

    return pattern;
}

/**
 * Check if a filename should be synchronized based on include and exclude patterns.
 * @param filename The name of the file to check.
 * @param options The options structure containing the patterns.
 * @return 1 if the file should be synchronized, 0 otherwise.
 */
int can_sync_in_i_or_o_pattern(const char *filename, options_t *options)
{
    int loop = 0;
    int is_in_i_pattern = 0; // Flag for if filename matches any include pattern
    int is_in_o_pattern = 0;  // Flag for if filename matches any exclude pattern

    // Check against all include patterns
    for (loop = 0; loop < options->i_pattern_count; loop++)
    {
        if (regexec(options->i_pattern_list[loop], filename, 0, NULL, 0) == 0)
        {
            is_in_i_pattern = 1;
            break;
        }
    }

     // Check against all exclude patterns, if any are defined
    if (options->o_pattern_count > 0)
    {
        is_in_o_pattern = 0;
        for (loop = 0; loop < options->o_pattern_count; loop++)
        {
            if (regexec(options->o_pattern_list[loop], filename, 0, NULL, 0) == 0)
            {
                is_in_o_pattern = 1;
                break;
            }
        }
    }
    else
    {   
        // If no exclude patterns are defined, treat filename as if it's not in any exclude pattern
        is_in_o_pattern = 1;
    }
    // Only synchronize if filename is not in any include pattern AND is in any exclude pattern
    return (!is_in_i_pattern) && is_in_o_pattern;
}

