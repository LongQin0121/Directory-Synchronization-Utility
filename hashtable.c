#include "hashtable.h"

//This is block of code is  mainly  from the workshp provided by lecturer.Updated a bit.


//Calculate a hash value for a given string
uint32_t hash_string(const char *string)
{
    uint32_t hash = 0;

    while(*string != '\0') {
        hash = hash*33 + *string;
        ++string;
    }
    return hash;
}

// Allocate and initialize space for a new hashtable.
HASHTABLE *hashtable_new()
{
    HASHTABLE   *new = NULL;

    new = (HASHTABLE *)malloc(sizeof(HASHTABLE));
    memset(new, 0, sizeof(HASHTABLE));
    //use calloc to make sure that the pointer is intilised as NULL
    new->hash_table = (LIST **) calloc(HASHTABLE_SIZE, sizeof(LIST *));
    CHECK_ALLOC(new->hash_table);
    new->file_info_list = allocate_file_info_list();

    return new;
}

//Release memory occupied
void hashtable_delete(HASHTABLE *hash_table)
{
    int loop = 0;

    if (NULL != hash_table)
    {
        if (NULL != hash_table->hash_table)
        {
            for (loop = 0; loop < HASHTABLE_SIZE; loop++)
            {
                list_delete(hash_table->hash_table[loop]);
            }
            free(hash_table->hash_table);
        }
        free_file_info_list(hash_table->file_info_list);

        free(hash_table);
    }
}

// Copy the contents from a source hashtable to a destination hashtable
void hashtable_copy(HASHTABLE *dst, HASHTABLE *src)
{
    node_t *node = NULL;

    node = hashtable_get_first_file_info(src);
    while (NULL != node)
    {
        hashtable_add(dst, construct_file_info(node->file_info), 1);
        node = node->next;
    }
}

//Add a new file_info_t to the hashtable.
void hashtable_add(HASHTABLE *hashtable, file_info_t *file_info, int insert_mode)
{
    uint32_t h   = hash_string(file_info->filepath) % HASHTABLE_SIZE;    // choose list

    hashtable->hash_table[h] = list_add(hashtable->hash_table[h], file_info);
    if (1 == insert_mode)
    {
        insert_file_info_mode1(hashtable->file_info_list, file_info);
    }
    else if (2 == insert_mode)
    {
        insert_file_info_mode2(hashtable->file_info_list, file_info);
    }
    else if (3 == insert_mode)
    {
        insert_file_info_mode3(hashtable->file_info_list, file_info);
    }
}

// Check if a specific string already exists in the hashtable.
LIST *hashtable_find(HASHTABLE *hashtable, char *string)
{
    uint32_t h = hash_string(string) % HASHTABLE_SIZE;     // choose list

    return list_find(hashtable->hash_table[h], string);
}

//Retrieve the first file info node from the hashtable
node_t *hashtable_get_first_file_info(HASHTABLE *hashtable)
{
    return hashtable->file_info_list->head;
}


