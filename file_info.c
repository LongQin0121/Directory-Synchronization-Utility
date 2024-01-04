#include "file_info.h"
#include "string_utility.h"

// Allocates and initializes memory for a new `file_info_t` structure.
file_info_t *allocate_file_info(struct stat *s, const char *dir,const char *filename,
                                int directory_index,int level,int is_directory)
{
    file_info_t *file_info = NULL;
    // Allocate memory for file_info structure.
    file_info = (file_info_t *)malloc(sizeof(file_info_t));

    if (file_info == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);  // Exit the program
    }

    memset(file_info, 0, sizeof(file_info_t));
    // Assign the provided values to the corresponding fields of the structure.
    file_info->directory_index = directory_index;
    file_info->level = level;
    file_info->is_directory = is_directory;

    // Copy the filename and concatenate the directory with filename to get the full path
    file_info->filename = copy_string(filename);
    file_info->filepath = concate_string(dir, filename);
    if (NULL != s)
    {
        file_info->st_mtime = s->st_mtime;  // Set the modification time from the stat structure.
        file_info->st_mode = s->st_mode;   // Set the mode (permissions) 
    }

    return file_info;
}

// Creates a new `file_info_t` structure and copies the content of an existing one.
file_info_t *construct_file_info(file_info_t *src)
{
    file_info_t *file_info = NULL;

    file_info = (file_info_t *)malloc(sizeof(file_info_t));
    if (file_info == NULL) {
        perror("Failed to allocate memory ");
        exit(EXIT_FAILURE);  // Exit the program
    }
    memset(file_info, 0, sizeof(file_info_t));
    copy_file_info(file_info, src);

    return file_info;
}

// Copies content of one `file_info_t` structure to another.
void copy_file_info(file_info_t *dst, file_info_t *src)
{   
     // If the destination structure already has a filename, free the associated memory.
    if (NULL != dst->filename)
    {
        free(dst->filename);
    }
     // If the destination structure already has a filepath, free the associated memory.
    if (NULL != dst->filepath)
    {
        free(dst->filepath);
    }
    // Copy fields from the source to the destination structure.
    dst->directory_index = src->directory_index;
    dst->level = src->level;
    dst->is_directory = src->is_directory;
    dst->st_mtime = src->st_mtime;
    dst->st_mode = src->st_mode;

    // Copy the filename and path from the source to the destination using the utility function.
    dst->filename = copy_string(src->filename);
    dst->filepath = copy_string(src->filepath);
}

// Frees memory associated with a given `file_info_t` structure.
void free_file_info(file_info_t *file_info)
{
    if (NULL != file_info)
    {
        if (NULL != file_info->filename)
        {
            free(file_info->filename);
        }
        if (NULL != file_info->filepath)
        {
            free(file_info->filepath);
        }
        free(file_info);
    }
}

// Compares two `file_info_t` structures based on filename and filepath.
// Returns negative, zero, or positive based on the comparison.
int compare_file_info(file_info_t *file_info1, file_info_t *file_info2)
{
    int compare_result = 0;

    compare_result = compare_string(file_info1->filename, file_info2->filename);
    if (0 == compare_result)
    {
        compare_result = compare_string(file_info1->filepath, file_info2->filepath);
    }
    return compare_result;
}

