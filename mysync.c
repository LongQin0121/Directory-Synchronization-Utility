#include "mysync.h"

int main(int argc, char *argv[])
{
    int loop = 0;
    options_t *options = NULL;
    HASHTABLE *sync_file_list_hash_table = NULL;
    HASHTABLE **file_list_hash_table_before_sync = NULL;
    HASHTABLE **file_list_hash_table_after_sync = NULL;

    // Parse command-line to get the user's options.
    options = parse_options(argc, argv);
    if (NULL == options)
    {
        exit(EXIT_FAILURE);
    }

    // Create a new hash table to store files that need syncing.
    sync_file_list_hash_table = hashtable_new();

    // Allocate memory for the hash tables to store file lists before synchronization.
    file_list_hash_table_before_sync = (HASHTABLE **)malloc(sizeof(HASHTABLE *) * options->directory_count);
    if (file_list_hash_table_before_sync == NULL) {
        perror("Failed to allocate memory for file_list_hash_table_before_sync");
        exit(EXIT_FAILURE);
    }
    
    // Initialize hash tables and populate them with the list of files to sync.
    for (loop = 0; loop < options->directory_count; loop++)
    {
        file_list_hash_table_before_sync[loop] = hashtable_new();
    }
    for (loop = 0; loop < options->directory_count; loop++)
    {
        get_sync_file_list("",options,sync_file_list_hash_table,
        file_list_hash_table_before_sync[loop],loop,0);
    }

    // Allocate memory for the hash tables ,and initialize it to store file lists after synchronization.
    file_list_hash_table_after_sync = (HASHTABLE **)malloc(sizeof(HASHTABLE *) * options->directory_count);
    if (file_list_hash_table_after_sync == NULL) {
    perror("Failed to allocate memory for file_list_hash_table_after_sync");
    exit(EXIT_FAILURE);
    }

    for (loop = 0; loop < options->directory_count; loop++)
    {
        file_list_hash_table_after_sync[loop] = hashtable_new();
    }
    
    // Start the synchronization process.
    sync(options, sync_file_list_hash_table, file_list_hash_table_before_sync, file_list_hash_table_after_sync);

    // Cleanup: delete the hash tables and free any allocated memory.
    hashtable_delete(sync_file_list_hash_table);
    for (loop = 0; loop < options->directory_count; loop++)
    {
        hashtable_delete(file_list_hash_table_before_sync[loop]);
        hashtable_delete(file_list_hash_table_after_sync[loop]);
    }
    free(file_list_hash_table_before_sync);
    free(file_list_hash_table_after_sync);

    free_options(options);

    return 0;
}

//remind the error input argument
void print_usage(const char *program_name)
{   
    fprintf(stderr, "%s [options] directory1 directory2 [directory3 ...]\n",program_name);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t-a\tSynchronize all files (including hidden).\n");
    fprintf(stderr, "\t-i pattern\tIgnore files matching the pattern.\n");
    fprintf(stderr, "\t-n\tPreview actions without making changes (also sets -v).\n");
    fprintf(stderr, "\t-o pattern\tOnly synchronize files matching the pattern.\n");
    fprintf(stderr, "\t-p\tPreserve file attributes.\n");
    fprintf(stderr, "\t-r\tSynchronize directories recursively.\n");
    fprintf(stderr, "\t-v\tVerbose output.\n");
    exit(EXIT_FAILURE);  // Exit with an error code
}

// Parse command-line options
options_t *parse_options(int argc, char *argv[])
{
    int opt = 0;
    int loop = 0;
    int i_pattern_ount = 0;
    int o_pattern_ount = 0;
    char error_string[256];
    options_t *options = NULL;
    DIR *dir = NULL;

    // Parse command-line options to count patterns
    while ((opt = getopt(argc, argv, "ai:no:prv")) != -1) {
        switch (opt) {
        case 'a':
            break;
        case 'i':
            i_pattern_ount++; // Count of patterns passed with '-i' option
            break;
        case 'n':
            break;
        case 'o':
            o_pattern_ount++;// Count of patterns passed with '-o' option
            break;
        case 'p':
            break;
        case 'r':
            break;
        case 'v':
            break;
        default:
            print_usage(argv[0]);
            return NULL;
        }
    }
     // Check if the minimum number of directories have been provided
    if (optind + 2 > argc)
    {
        printf(" To sync, we need at least two directory.\n");
        print_usage(argv[0]);
        return NULL;
    }

    // Allocate memory for the options structure with counts of patterns and directories
    options = allocate_options(i_pattern_ount, o_pattern_ount, argc - optind);
    
    // Reset pattern counters and option index
    i_pattern_ount = 0;
    o_pattern_ount = 0;
    optind = 1;

    // Parse command-line options again to fill the options structure
    while ((opt = getopt(argc, argv, "ai:no:prv")) != -1) {
        switch (opt) {
        case 'a':
            options->a_flag = 1;
            break;
        case 'i':
            // Compile and store the '-i' pattern
            options->i_pattern_list[i_pattern_ount] = compile_pattern(optarg);
            if (NULL == options->i_pattern_list[i_pattern_ount])
            {
                sprintf(error_string, "Compile -i pattern %s error\n", optarg);
                perror(error_string);
                free_options(options);
                return NULL;
            }
            i_pattern_ount++;
            break;
        case 'n':
            options->n_flag = 1;
            options->v_flag = 1;
            break;
        case 'o':
            // Compile and store the '-o' pattern
            options->o_pattern_list[o_pattern_ount] = compile_pattern(optarg);
            if (NULL == options->o_pattern_list[o_pattern_ount])
            {
                sprintf(error_string, "Compile -o pattern %s error\n", optarg);
                perror(error_string);
                free_options(options);
                return NULL;
            }
            o_pattern_ount++;
            break;
        case 'p':
            options->p_flag = 1;
            break;
        case 'r':
            options->r_flag = 1;
            break;
        case 'v':
            options->v_flag = 1;
            break;
        default:
            print_usage(argv[0]);
            free_options(options);
            return NULL;
        }
    }
    // Check if the specified directories exist
    for (loop = optind; loop < argc; loop++)
    {
        dir = opendir(argv[loop]);
        if (NULL == dir)
        {
            sprintf(error_string, "Directory %s to sync does not exist!\n", argv[loop]);
            perror(error_string);
            free_options(options);
            return NULL;
        }
        closedir(dir);
    }

     // Store the specified directories in the options structure
    for (loop = optind; loop < argc; loop++)
    {
        options->directory_list[loop - optind] = copy_string(argv[loop]);
    }

    return options;
}

