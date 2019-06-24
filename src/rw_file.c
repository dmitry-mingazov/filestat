#include "rw_file.h"

input_file_argument *readInputFile(char *path)
{
  FILE *fp;
  fp = fopen(path, "r");
  // printf("Input file: %s\n", path);
  if(fp == NULL){
    fprintf(stderr, "Cannot open %s, exiting. . .\n", path);
    exit(1);
  }

  char *buf;
  size_t n = 0;
  size_t bufsize = 0;
  int i;
  char opts[PATH_ARGUMENTS];

  input_file_argument *root = NULL;
  input_file_argument *prev = NULL;
  input_file_argument *curr = NULL;

  while((bufsize = getline(&buf, &n, fp)) != -1){
    if(root == NULL){
      root = malloc(sizeof(input_file_argument));
      curr = root;
    }else{
      curr = malloc(sizeof(input_file_argument));
      prev->next = curr;
    }

    for(i = 0; i < PATH_ARGUMENTS; i++)
      opts[i] = 0;

    curr->options = 00;
    //TODO: make it upgradable
    // printf("Buffer: %s", buf);
    curr->path = malloc(sizeof(char) * bufsize);
    if(sscanf(buf, "%s %c %c", curr->path, &opts[0], &opts[1]) < 1){
      //if this line is empty or has only "space" characters, ignore it
      continue;
      // fprintf(stderr, "SSCANF ERROR\n");
    }
    // printf("rw_file: curr->path = %s\n", curr->path);
    for(i = 0; i < PATH_ARGUMENTS; i++){
      switch(opts[i]){
        case 'r':
          curr->options |= RECURSIVE;
          break;
        case 'l':
          curr->options |= FOLLOW_LINK;
          break;
        case 0:
          break;
        default:
          fprintf(stderr, "Wrong input file format, exiting. . .\n");
          exit(1);
      }
    }
  // printf("rw_file: %s has %o opts\n", curr->path, curr->options);
  prev = curr;

  }
  return root;
}

void writeOutputFile(char *file_path)
{
  FILE *fp;
  fp = fopen(file_path, "w");

  if(fp == NULL){
    fprintf(stderr, "Cannot open %s, exiting. . .\n", file_path);
    exit(1);
  }
  scanned_path *tmp_path;
  file_info *tmp_info;
  while((tmp_path = rbnext()) != NULL){
    fprintf(fp, "# %s\n", tmp_path->path);
    tmp_info = tmp_path->head;
    while(tmp_info !=  NULL){
      fprintf(fp, "%ld %d %d %ld %o %ld %ld %ld %ld\n",
              tmp_info->date,
              tmp_info->uid,
              tmp_info->gid,
              tmp_info->size,
              tmp_info->mode,
              tmp_info->atime,
              tmp_info->ctime,
              tmp_info->mtime,
              tmp_info->nlink);
      tmp_info = tmp_info->next;
    }
    fprintf(fp, "###\n");
  }
  fprintf(fp, "###\n");
  fclose(fp);

}
