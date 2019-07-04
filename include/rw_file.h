#ifndef RW_FILE
#define RW_FILE

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "rbtree.h"

#define RECURSIVE 01
#define FOLLOW_LINK 02

#define PATH_ARGUMENTS 2


input_file_argument *readInputFile(char *path);
void writeOutputFile(char *file_path, scanned_path **pathlist, long int treesize);
void readOutputFile(char *file_path, tree_descriptor *tree);

#endif
