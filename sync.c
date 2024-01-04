#include "sync.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utime.h>
#include <unistd.h>
#include "string_utility.h"

/**
 * Populates the provided hash tables with files and directories that 
 * need to be synchronized.
 * @param dir_path Current directory path.
 * @param options Contains the include and exclude patterns, flags, and directory list.
 * @param sync_file_list_hash_table Hash table to be populated with files that need synchronization.
 * @param file_list_hash_table_before_sync Hash table with the current file state.
 * @param directory_index Index of the directory in the options directory list.
 * @param level Current directory depth level.
 */
void get_sync_file_list(const char *dir_path,options_t *options,
    HASHTABLE *sync_file_list_hash_table,HASHTABLE *file_list_hash_table_before_sync,
    int directory_index,int level)
{
    DIR *dir = NULL;
    struct dirent *directory_entry = NULL;
    char *sub_dir_path = NULL;
    char *filename = NULL;
    char *filepath = NULL;
    int stat_result = 0;
    struct stat s;
    file_info_t *file_info = NULL;
    LIST *list = NULL;

    // Create a full path using the base directory and the provided path
    sub_dir_path = concate_string(options->directory_list[directory_index], dir_path);
    dir = opendir(sub_dir_path);
     // Iterate over each entry in the directory
    while (NULL != (directory_entry = readdir(dir)))
    {   
        // Skip the current and parent directory entries
        if ((0 == strcmp(".", directory_entry->d_name))
            || (0 == strcmp("..", directory_entry->d_name)))
        {
            continue;
        }
        // If the 'a' flag isn't set, skip hidden files
        if (!options->a_flag && ('.' == directory_entry->d_name[0]))
        {
            continue;
        }
        // Create a full path for the current directory entry
        filepath = concate_string(sub_dir_path, directory_entry->d_name);
        stat_result = stat(filepath, &s);
        free(filepath);

        if (0 != stat_result)// If we failed to get the file info, skip this entry
        {
            continue;
        }

        // Create a relative path for the current directory entry
        filename = concate_string(dir_path, directory_entry->d_name);

        // If the current entry is a directory
        if (S_ISDIR(s.st_mode))
        {
            if (options->r_flag)// If recursive flag is set, process this directory recursively
            {
                file_info = allocate_file_info(&s,dir_path,
                    directory_entry->d_name,directory_index,level, 1);

                // Add the directory to the hash table representing the state before synchronization
                hashtable_add(file_list_hash_table_before_sync, file_info, 1);
                // Recursive call to process this directory
                get_sync_file_list(filename,options,sync_file_list_hash_table,
                    file_list_hash_table_before_sync,directory_index,level + 1);
            }
        }

        // If the current directory entry is a file
        else if (S_ISREG(s.st_mode))
        {   
            // Check if the file matches the inclusion or exclusion patterns (if any)
            if (can_sync_in_i_or_o_pattern(directory_entry->d_name, options))
            {
                file_info = allocate_file_info(&s,dir_path,
                    directory_entry->d_name,directory_index,level,0);
                // Add the file to the hash table representing the state before synchronization
                hashtable_add(file_list_hash_table_before_sync, file_info, 1);

                // Check if the file is already in the synchronization list
                list = hashtable_find(sync_file_list_hash_table, filename);
                if (NULL == list)
                {
                    hashtable_add(sync_file_list_hash_table, construct_file_info(file_info), 3);
                }
                else
                {   
                    // If the file in the synchronization list is older, update its information
                    if (list->file_info->st_mtime < file_info->st_mtime)
                    {
                        copy_file_info(list->file_info, file_info);
                    }
                }
            }
        }
        free(filename);
    }
    closedir(dir);
    free(sub_dir_path);
}

//Main function to synchronize files across directories.
void sync(options_t *options,
    HASHTABLE *sync_file_list_hash_table, HASHTABLE **file_list_hash_table_before_sync,
    HASHTABLE **file_list_hash_table_after_sync)
{
    int loop = 0;  // Loop counter.
    node_t *node = NULL;  // Pointer to traverse the file_info_list in the hash table.

    // For each directory specified in the options.
    for (loop = 0; loop < options->directory_count; loop++)
    {   
        // Copy the contents of the 'before' hash table to the 'after' hash table for each directory.
        // This sets up an initial state for the 'after' sync hash table.
        hashtable_copy(file_list_hash_table_after_sync[loop],
            file_list_hash_table_before_sync[loop]);
    }
    // Begin processing the list of files that need to be synchronized.
    node = sync_file_list_hash_table->file_info_list->head;
    while (NULL != node)
    {
        for (loop = 0; loop < options->directory_count; loop++)
        {   
            // If the current directory is the source directory for the current file, skip it.
            // This ensures that a file isn't synchronized with itself.
            if (loop == node->file_info->directory_index)
            {
                continue;
            }
            // Perform synchronization for the current file.
            // The function sync_file will handle the actual copying/updating of the file.
            sync_file(node->file_info,loop,node->file_info->directory_index,
                options,file_list_hash_table_after_sync[loop]);
        }
        node = node->next; // Move to the next file in the sync list.
    }
}


//Handles the synchronization of a single file.
void sync_file(file_info_t *file_info,int dst_dir_index,int src_dir_index,
    options_t *options, HASHTABLE *hash_table)
{
    time_t current_time;   // Holds the current system time.
    LIST *list = NULL;     // List to hold file information.
    file_info_t *dst_file_info = NULL;  // Information about the destination file.
    current_time = time(NULL);  
    
    // If the verbose flag is set, print the copy operation details.
    if (options->v_flag)
    {
        printf("Copy: '%s/%s' to '%s/%s'\n",
            options->directory_list[src_dir_index],file_info->filepath,
            options->directory_list[dst_dir_index],file_info->filepath);
    }

    // Ensure all leading directories exist in the destination directory path.
    make_all_leading_directories(file_info->filepath,dst_dir_index,options, hash_table);

    // Find if the file already exists in the destination hash table.
    list = hashtable_find(hash_table, file_info->filepath);

    // If the file does not exist, construct a new file info for the destination.
    if (NULL == list)
    {  
        dst_file_info = construct_file_info(file_info);
        dst_file_info->directory_index = dst_dir_index;
        // Add the new file info to the destination hash table.
        hashtable_add(hash_table, dst_file_info, 2);
    }
    else
    {   
        // If the file already exists, fetch its information.
        dst_file_info = list->file_info;
    }
    // If the preserve flag is set, maintain the source file's metadata.
    if (options->p_flag)
    {
        dst_file_info->st_mtime = file_info->st_mtime;
        dst_file_info->st_mode = file_info->st_mode;
    }
    else
    {   
        // Otherwise, set the destination file's metadata to default values.
        dst_file_info->st_mtime = current_time;
        dst_file_info->st_mode = 0666;   // Default file permissions.
    }
     // If the no-overwrite flag is NOT set, perform the file copy.
    if (!options->n_flag)
    {
        copy_file(file_info,dst_dir_index,src_dir_index,options,current_time);

        if (options->p_flag)
        {
        }
    }
}

//Physically copies a file from the source to the destination directory.
void copy_file(file_info_t *file_info,int dst_dir_index,int src_dir_index,
    options_t *options,time_t current_time)
{
    char *dst_path = NULL;
    char *src_path = NULL;
    int dst_fd = 0;
    int src_fd = 0;
    int read_data_length = 0;
    char *buffer = NULL;
    struct utimbuf timbuf;

    buffer = (char *)malloc(6000);

     // Construct source and destination file paths.
    dst_path = concate_string(options->directory_list[dst_dir_index],file_info->filepath);
    src_path = concate_string(options->directory_list[src_dir_index],file_info->filepath);

    // Open source file for reading and destination file for writing.
    dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    src_fd = open(src_path, O_RDONLY);
    while (0 < (read_data_length = read(src_fd, buffer, 6000)))
    {   
        // Write data from source to destination.
        write(dst_fd, buffer, read_data_length);
    }
    close(dst_fd);
    close(src_fd);

    // If -p is set, update the modification time and permissions for the copied file.
    if (options->p_flag)
    {
        timbuf.modtime = file_info->st_mtime;
        timbuf.actime = current_time;
        utime(dst_path, &timbuf);

        chmod(dst_path, file_info->st_mode);
    }

    free(buffer);
    free(dst_path);
    free(src_path);
}

//Creates all leading directories for a given filename.
void make_all_leading_directories(const char *filename,int dst_dir_index,
    options_t *options,HASHTABLE *hash_table)
{
    int level = 0;
    char *dir_path = NULL;
    char *sub_dir_path = NULL;
    char *dirname = NULL;
    char *split_char = NULL;

    // Create a copy of the filename.
    sub_dir_path = copy_string(filename);
    dirname = sub_dir_path;
    while (1)
    {
        split_char = strstr(dirname, "/");
        if (NULL == split_char)
        {
            break;
        }
        *split_char = '\0';

        // Check if the directory already exists in the hash table.
        if (NULL == hashtable_find(hash_table, sub_dir_path))
        {   
            // Construct full path for the current directory.
            dir_path = concate_string(options->directory_list[dst_dir_index], sub_dir_path);
            if (level > 0)
            {
                *(dirname - 1) = '\0';
            }
            // Create the directory and its metadata.
            make_directories(dir_path, sub_dir_path, dirname, dst_dir_index, level,
                options,hash_table);
            if (level > 0)
            {
                *(dirname - 1) = '/';
            }
            free(dir_path);
        }
          // Move to the next directory in the path.
        *split_char = '/';
        dirname = split_char + 1;
        level++;
    }
    free(sub_dir_path);
}

//Creates directories based on the provided path.
void make_directories(const char *dir_path,const char *sub_dir_path,const char *dirname,
    int dst_dir_index,int level,options_t *options,HASHTABLE *hash_table)
{   
    // If it is top level,add the directory info to the hash table without a parent directory.
    if (0 == level)
    {
        hashtable_add(hash_table,
            allocate_file_info(NULL, "", dirname, dst_dir_index, level, 1),2);
    }
    else  // Add the directory info to the hash table with its parent directory.
    {   
        hashtable_add(hash_table,
            allocate_file_info(NULL, sub_dir_path, dirname, dst_dir_index, level, 1), 2);
    }
     // If the 'no-action' flag is not set, create the directory.
    if (!options->n_flag)
    {
        mkdir(dir_path, 0777);
    }
}
