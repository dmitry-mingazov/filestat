#ifndef RW_FILE
#define RW_FILE

#include <stdio.h>
#include <stdlib.h>

#define RECURSIVE 01
#define FOLLOW_LINK 02

#define PATH_ARGUMENTS 2

typedef struct s_input_file_argument {
  char *path;
  unsigned char options;
  struct s_input_file_argument *next;
} input_file_argument;

input_file_argument *readInputFile(char *path);
void writeOutputFile(char *file_path);

#endif
