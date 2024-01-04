#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "options.h"
#include "string_utility.h"
#include "sync.h"





// Function prototype declarations，remind argument format
void print_usage(const char *program_name);

// Parses and processes the command line arguments.
options_t *parse_options(int argc, char *argv[]);
