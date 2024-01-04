#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "file_info.h"

//  A HELPFUL PREPROCESSOR MACRO TO CHECK IF ALLOCATIONS WERE SUCCESSFUL
#define CHECK_ALLOC(p) if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }

//A POINTER TO ANOTHER LIST
typedef struct _list {
     file_info_t    *file_info;
     struct _list   *next;
} LIST;


extern	LIST	*list_new(void);

//Frees all the nodes of the provided linked list.
extern  void    list_delete(LIST *list);

//Insert a new item.
extern	LIST	*list_add(LIST *list, file_info_t *file_info);

// Search for a specific item in the list based on its filepath.
extern	LIST	*list_find (LIST *list, char *wanted);


#endif





