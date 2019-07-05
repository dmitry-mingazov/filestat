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
      //free allocated memory
      free(curr->path);
      free(curr);
      curr = NULL;
      //if prev doesn't exist, it must be the first iteration
      if(prev == NULL){
        root = NULL;
      }
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
          fprintf(stderr, "rwfile: wrong input file format, exiting. . .\n");
          exit(1);
      }
    }
  // printf("rw_file: %s has %o opts\n", curr->path, curr->options);
  prev = curr;

  }

  if(root == NULL){
    fprintf(stderr, "Empty input file, exiting. . .\n");
    exit(1);
  }

  return root;
}

void readOutputFile(char *file_path, tree_descriptor *tree)
{
  FILE *fp;
  fp = fopen(file_path, "r");

  if(fp == NULL){
    return;
  }

  scanned_path *tmp_path;
  file_info *tmp_info;

  size_t n = 0;
  size_t bufsize = 0;
  char *buf;

  int pound = 0;

  // while(endfile){
  //   c = getc(fp);
  //   if(c == EOF){
  //     return;
  //   }
  //   if(c == '#'){
  //     pound++;
  //     continue;
  //   }
  //   pound--;
  //   if(c == '\n'){
  //
  //   }
  // }
  treenode_data *t_data;
  union u_treenode_data data;

  while((bufsize = getline(&buf, &n, fp)) != -1){
    //if is path name, analyze it
    if(buf[0] == '#'){
      if(buf[1] != '#'){
        tmp_path = (scanned_path*) malloc(sizeof(scanned_path));
        tmp_path->head = NULL;
        tmp_path->tail = NULL;
        tmp_path->path = (char*) malloc(sizeof(char) * (bufsize - 1));
        tmp_path->status = READ_OUTPUT;
        sscanf(buf, "# %s", tmp_path->path);

        data.file = tmp_path;

        t_data = (treenode_data*) malloc(sizeof(treenode_data));
        t_data->type = T_SCANNED_PATH;
        t_data->data = data;
        continue;
      }else{
        pound++;
        if(pound < 2){
          add_rbtree(tree, &t_data);
          continue;
        }else{
          break;
        }
      }
    }
    pound = 0;
    tmp_info = (file_info*) malloc(sizeof(file_info));
    if(sscanf(buf, "%ld %d %d %ld %o %ld %ld %ld %ld\n",
      &tmp_info->date,
      &tmp_info->uid,
      &tmp_info->gid,
      &tmp_info->size,
      &tmp_info->mode,
      &tmp_info->atime,
      &tmp_info->ctime,
      &tmp_info->mtime,
      &tmp_info->nlink) <= 0){
        free(tmp_info);
        continue;
            }

    if(tmp_path->head == NULL){
      tmp_path->head = tmp_info;
      tmp_path->tail = tmp_info;
    }else{
      tmp_path->tail->next = tmp_info;
      tmp_path->tail = tmp_info;
    }
  }
}

void writeOutputFile(char *file_path, scanned_path **pathlist, long int treesize)
{
  FILE *fp;
  fp = fopen(file_path, "w");

  if(fp == NULL){
    fprintf(stderr, "Cannot open %s, exiting. . .\n", file_path);
    exit(1);
  }
  // scanned_path *tmp_path;
  file_info *tmp_info;
  // while((tmp_path = rbnext()) != NULL){
  for(int i = 0; i < treesize ; i++){
    fprintf(fp, "# %s\n", pathlist[i]->path);
    tmp_info = pathlist[i]->head;
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
