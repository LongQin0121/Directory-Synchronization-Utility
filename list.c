#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// Ensure the availability of the strdup function on Linux systems.
#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

//This is block of code is  mainly  from the workshop provided by lecturer.Updated a bit.

// Initialize a new list
LIST *list_new(void)
{
    return NULL;
}

//Frees all the nodes of the provided linked list.
void list_delete(LIST *list)
{
    LIST *cur = NULL;
    LIST *next = NULL;

    cur = list;
    while (NULL != cur)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }
}

// Search for a specific item in the list based on its filepath.
LIST *list_find(LIST *list, char *wanted)
{
    while(list != NULL) {
	if(strcmp(list->file_info->filepath, wanted) == 0) {
	    return list;
	}
	list	= list->next;
    }
    return NULL;
}

//Allocate and initialize a new list node with the provided file_info.
LIST *list_new_item(file_info_t *file_info)
{
    LIST *new = calloc(1, sizeof(LIST) );
    if (new == NULL) {
        perror("calloc");
    }
    CHECK_ALLOC(new);
    new->file_info  =  file_info;
    new->next  =  NULL;
    return new;
}

//Insert a new item.
LIST *list_add(LIST *list, file_info_t *file_info)
{
    LIST *new   = list_new_item(file_info);
    new->next   = list;
    return new;
}