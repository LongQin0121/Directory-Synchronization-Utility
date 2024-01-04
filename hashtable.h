#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "list.h"
#include "file_info_list.h"

#define	HASHTABLE_SIZE		997

// Provided by workshop, modified a bit!

// It contains an array of lists representing the table itself 
// and an associated file info list for additional meta information.
typedef	struct
{
    LIST **hash_table;
    file_info_list_t *file_info_list;
} HASHTABLE;

//-------  Function declarations --------

//create,delete and  Copy contents of one hashtable to another
extern	HASHTABLE	*hashtable_new();
void                hashtable_delete(HASHTABLE *hash_table);
void                hashtable_copy(HASHTABLE *dst, HASHTABLE *src);

// Add a file_info_t entry to the hashtable.
extern	void		 hashtable_add(HASHTABLE *, file_info_t *file_info, int insert_mode);

// Search for a string in the hashtable.
extern	LIST		 *hashtable_find(HASHTABLE *, char *string);

// Retrieve the first file info from the hashtable.
extern	node_t		 *hashtable_get_first_file_info(HASHTABLE *);


#endif




