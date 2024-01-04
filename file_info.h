#ifndef FILE_INFO_H
#define FILE_INFO_H
#include <time.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    int directory_index;  // An index representing the directory's position
    int level;              // The depth or level of the file/directory in the file hierarchy
    int is_directory;   // Flag to indicate if the current item is a directory (1) or file (0)
    time_t st_mtime;      // The modification time of the file
    mode_t st_mode;        // File mode (permissions and type)
    char *filename;        // The name of the file or directory
    char *filepath;          // The full path to the file or directory
} file_info_t;

// Function prototypes

// Allocates and initializes memory for a new `file_info_t` structure
file_info_t *allocate_file_info(struct stat *s,
    const char *dir,
    const char *filename,
    int directory_index,
    int level,
    int is_directory);

// Allocates and initializes memory for a new `file_info_t` structure
file_info_t *construct_file_info(file_info_t *src);

// Copies content of one `file_info_t` structure to another.
void copy_file_info(file_info_t *dst, file_info_t *src);

void free_file_info(file_info_t *file_info);

// Compares two `file_info_t` structures based on filename and filepath.
// Returns negative, zero, or positive based on the comparison.
int compare_file_info(file_info_t *file_info1, file_info_t *file_info2);

#endif


