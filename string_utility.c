#include "string_utility.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Allocates memory and copies the given string into it.
char *copy_string(const char *str)
{
    int string_length = 0;
    char *new_string = NULL;

    if (NULL != str)
    {
        string_length = (int)strlen(str);
    }
    // If the string is not empty, copy it
    if (string_length > 0)
    {
        new_string = (char *)malloc(sizeof(char) * (string_length + 1));
        if (new_string == NULL) {
        perror("Failed to allocate memory for new_string");
        exit(EXIT_FAILURE);
    }

        strcpy(new_string, str);
    }

    return new_string;
}

/**
 * Concatenates two strings with a '/' between them.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return Returns a new string which is the concatenation of str1 and str2 with a '/' in between.
 */
char *concate_string(const char *str1, const char *str2)
{
    int string_length1 = 0;
    int string_length2 = 0;
    char *new_string = NULL;

    string_length1 = (int)strlen(str1);
    string_length2 = (int)strlen(str2);
    new_string = (char *)malloc(sizeof(char) * (string_length1 + strlen("/") + string_length2 + 1));
    if (new_string == NULL) {
        perror("Failed to allocate memory for new_string");
        exit(EXIT_FAILURE);
    }

    if (string_length1 > 0)
    {
        strcpy(new_string, str1);
        if (string_length2 > 0)
        {
            strcat(new_string, "/");
            strcat(new_string, str2);
        }
    }
    else
    {
        strcpy(new_string, str2);
    }

    return new_string;
}


/**
 * Compares two characters, taking special precedence for the dot ('.') character.
 * @return Returns positive if ch1 is greater, negative if ch2 is greater, and 0 if they are equal.
 */
int compare_char(char ch1, char ch2)
{
    if (('.' == ch1) && ('.' != ch2))
    {
        return 1;
    }
    else if (('.' != ch1) && ('.' == ch2))
    {
        return -1;
    }
    if (ch1 > ch2)
    {
        return 1;
    }
    else if (ch1 < ch2)
    {
        return -1;
    }

    return 0;
}

/**
 * Compares two strings character by character, considering special precedence for the dot ('.') character.
 *
 * @param str1 The first string.
 * @param str2 The second string.
 * @return Returns positive if str1 is greater, negative if str2 is greater, and 0 if they are equal.
 *///Compares two strings character by character, considering special precedence for the dot ('.') character.
int compare_string(const char *str1, const char *str2)
{
    int loop = 0;
    int compare_result = 0;
    int string_length1 = 0;
    int string_length2 = 0;

    string_length1 = (int)strlen(str1);
    string_length2 = (int)strlen(str2);
    while ((loop < string_length1) && (loop < string_length2))
    {
        compare_result = compare_char(str1[loop], str2[loop]);
        if (0 != compare_result)
        {
            return compare_result;
        }
        loop++;
    }
    if (string_length1 < string_length2)
    {
        return -1;
    }
    else if (string_length1 > string_length2)
    {
        return 1;
    }

    return 0;
}
