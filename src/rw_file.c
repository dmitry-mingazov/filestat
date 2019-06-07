#include "rw_file.h"

void readInputFile(char *path, input_file_argument *input_args)
{
  //Testing
  input_args->options = 00;
  input_args->path = "src";
  input_args->options |= RECURSIVE;
  input_args->next = NULL;

  printf("\nInput file: %s\n", path);
}
