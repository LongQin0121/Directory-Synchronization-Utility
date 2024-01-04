#ifndef SYNC_H
#define SYNC_H

#include <sys/types.h>
#include <dirent.h>
#include "hashtable.h"
#include "file_info_list.h"
#include "options.h"

void get_sync_file_list(const char *dir_path,
    options_t *options,
    HASHTABLE *sync_file_list_hash_table,
    HASHTABLE *file_list_hash_table_before_sync,
    int directory_index,
    int level);
void sync(options_t *options,
    HASHTABLE *sync_file_list_hash_table,
    HASHTABLE **file_list_hash_table_before_sync,
    HASHTABLE **file_list_hash_table_after_sync);
void sync_file(file_info_t *file_info,
    int dst_dir_index,
    int src_dir_index,
    options_t *options,
    HASHTABLE *hash_table);
void copy_file(file_info_t *file_info,
    int dst_dir_index,
    int src_dir_index,
    options_t *options,
    time_t current_time);
void make_all_leading_directories(const char *filename,
    int dst_dir_index,
    options_t *options,
    HASHTABLE *hash_table);
void make_directories(const char *dir_path,
    const char *sub_dir_path,
    const char *dirname,
    int dst_dir_index,
    int level,
    options_t *options,
    HASHTABLE *hash_table);

#endif



