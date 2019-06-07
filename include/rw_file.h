#ifndef RW_FILE
#define RW_FILE

#include <stdio.h>

#define RECURSIVE 01
#define FOLLOW_LINK 02

typedef struct s_input_file_argument {
  char *path;
  unsigned char options;
  struct s_input_file_argument *next;
} input_file_argument;

void readInputFile(char *path, input_file_argument *input_args);

#endif
