#ifndef FILE_INFO_LIST_H
#define FILE_INFO_LIST_H
#include "file_info.h"

// Define a node structure for doubly-linked list.
// Each node contains information about a file or directory.
typedef struct _node
{
    file_info_t *file_info;
    struct _node *next;
    struct _node *prev;
} node_t;

// Define a structure for a doubly-linked list.
// The list keeps track of file and directory nodes and counts.
typedef struct{
    node_t *head;
    node_t *tail;
    int file_count;
    int directory_count;
} file_info_list_t;

// allocate memory for a new file_info_list_t structure.
file_info_list_t *allocate_file_info_list();

// free memory 
void free_file_info_list(file_info_list_t *file_info_list);

// insert a file_info into the list in append mode.
void insert_file_info_mode1(file_info_list_t *file_info_list, file_info_t *file_info);

//insert a file_info  in alphabetical order based on filepath.
void insert_file_info_mode2(file_info_list_t *file_info_list, file_info_t *file_info);

// insert a file_info based on a custom comparison function
void insert_file_info_mode3(file_info_list_t *file_info_list, file_info_t *file_info);

//increment the appropriate count (file or directory) in the list.
void inc_file_or_dir_count(file_info_list_t *file_info_list, file_info_t *file_info);

//allocate memory for a new node.
node_t *allocate_node(file_info_t *file_info);

void free_node(node_t *node);

#endif


