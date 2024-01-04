#include "file_info_list.h"

// Allocates memory for a new file_info_list_t structure and initializes it.
file_info_list_t *allocate_file_info_list()
{
    file_info_list_t *file_info_list = NULL;

    file_info_list = (file_info_list_t *)malloc(sizeof(file_info_list_t));
    if (file_info_list == NULL) {
        perror("Failed to allocate memory for file_info_list");
        exit(EXIT_FAILURE);  // Exit the program
    }
    memset(file_info_list, 0, sizeof(file_info_list_t));

    return file_info_list;
}

// Frees the memory of the entire file_info_list_t structure, including all its nodes.
void free_file_info_list(file_info_list_t *file_info_list)
{
    node_t *cur = NULL;
    node_t *next = NULL;

    if (NULL != file_info_list)
    {
        cur = file_info_list->head;
        while (NULL != cur)
        {
            next = cur->next;
            free_node(cur);
            cur = next;
        }
        free(file_info_list);
    }
}

// Inserts a new file_info into the file_info_list_t at the end - mode1
void insert_file_info_mode1(file_info_list_t *file_info_list, file_info_t *file_info)
{
    node_t *prev = NULL;
    node_t *node = NULL;

    node = allocate_node(file_info);
    prev = file_info_list->tail;
    node->prev = prev;
    node->next = NULL;
    if (NULL == prev)
    {
        file_info_list->head = node;
    }
    else
    {
        prev->next = node;
    }
    file_info_list->tail = node;
    inc_file_or_dir_count(file_info_list, file_info);
}

// Inserts a new file_info into the file_info_list_t in alphabetical order based on filepath.
void insert_file_info_mode2(file_info_list_t *file_info_list, file_info_t *file_info)
{
    node_t *prev = NULL;
    node_t *next = NULL;
    node_t *node = NULL;

    node = allocate_node(file_info);
    next = file_info_list->head;
    while (NULL != next)
    {
        if (strcmp(file_info->filepath, next->file_info->filepath) < 0)
        {
            break;
        }
        prev = next;
        next = prev->next;
    }
    node->prev = prev;
    node->next = next;
    if (NULL == prev)
    {
        file_info_list->head = node;
    }
    else
    {
        prev->next = node;
    }
    if (NULL == next)
    {
        file_info_list->tail = node;
    }
    else
    {
        next->prev = node;
    }
    inc_file_or_dir_count(file_info_list, file_info);
}

// Inserts a new file_info into the file_info_list_t based on custom compare_file_info function.
void insert_file_info_mode3(file_info_list_t *file_info_list, file_info_t *file_info)
{
    node_t *prev = NULL;
    node_t *next = NULL;
    node_t *node = NULL;

    node = allocate_node(file_info);
    next = file_info_list->head;
    while (NULL != next)
    {
        if (compare_file_info(file_info, next->file_info) < 0)
        {
            break;
        }
        prev = next;
        next = prev->next;
    }
    node->prev = prev;
    node->next = next;
    if (NULL == prev)
    {
        file_info_list->head = node;
    }
    else
    {
        prev->next = node;
    }
    if (NULL == next)
    {
        file_info_list->tail = node;
    }
    else
    {
        next->prev = node;
    }
    inc_file_or_dir_count(file_info_list, file_info);
}


// Increases the count of files or directories in the file_info_list_t based on the type of the provided file_info.
void inc_file_or_dir_count(file_info_list_t *file_info_list, file_info_t *file_info)
{
    if (file_info->is_directory)
    {
        file_info_list->directory_count++;
    }
    else
    {
        file_info_list->file_count++;
    }
}

// Allocates memory for a new node and initializes it with the provided file_info.
node_t *allocate_node(file_info_t *file_info)
{
    node_t *node = NULL;

    node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL) {
        perror("Failed to allocate memory for node");
        exit(EXIT_FAILURE);  // Exit the program
    }

    memset(node, 0, sizeof(node_t));
    node->file_info = file_info;

    return node;
}

// Frees the memory of a node and its associated file_info.
void free_node(node_t *node)
{
    if (NULL != node)
    {
        if (NULL != node->file_info)
        {
            free_file_info(node->file_info);
        }
        free(node);
    }
}


