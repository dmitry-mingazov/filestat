#include "filestat.h"

static filestat_configuration fsconf;
static tree_descriptor *tree;
static tree_descriptor *inode_tree;

int main(int argc, char **argv)
{
  filestat_configuration *pfsconf = &fsconf;
  getOptions(argc, argv, &pfsconf);

  input_file_argument *input_args = fsconf.input_args;
  input_file_argument *prev = input_args;

  tree = init_rbtree();
  inode_tree = init_rbtree();

  if(HASOPT(fsconf.hasopt, STAT))
    init_stats();
  if(HASOPT(fsconf.hasopt, REPORT))
    init_report();

  readOutputFile(pfsconf->output_file, tree);

  if(HASOPT(fsconf.hasopt, HISTORY)){
    treenode_data *history_data = filepath_to_treenode_data(fsconf.history);
    history_data = get_data_rbtree(tree, history_data);
    if(history_data != NULL){
      print_history_of_file(history_data->data.file);
    }else{
      fprintf(stderr, "filestat: no history found for %s\n", fsconf.history);
    }
  }

  long int treesize = 0;

  if(!(HASOPT(fsconf.hasopt, NOSCAN))){
    while(input_args != NULL){
      filestat(input_args);
      prev = input_args;
      input_args = input_args->next;
      free(prev->path);
      free(prev);
    }
  }

  scanned_path **pathlist = path_inorder(tree, &treesize);

  if(HASOPT(fsconf.hasopt, NOSCAN)){
    print_output_file(pathlist, treesize);
  }else{
    writeOutputFile(pfsconf->output_file, pathlist, treesize);
  }


  if(HASOPT(fsconf.hasopt, STAT))
    print_program_stats(end_stats());
  if(HASOPT(fsconf.hasopt, REPORT))
    print_program_report(end_report());
}

void filestat(input_file_argument *input_args)
{
  struct stat stbuf;
  file_info *fsbuf = (file_info*) malloc(sizeof(file_info));
  if(fsbuf == NULL){
    fprintf(stderr, "%s", MALLOC_ERROR);
    exit(1);
  }

  if(HASOPT(input_args->options, FOLLOW_LINK)){
    if(stat(input_args->path, &stbuf) == -1){
      fprintf(stderr, "filestat: can't access to %s\n", input_args->path);
      return;
    }
  }else{
    if(lstat(input_args->path, &stbuf) == -1){
      fprintf(stderr, "filestat: can't access to %s\n", input_args->path);
      return;
    }
  }

  *fsbuf = statcpy(&stbuf);

  treenode_data *pt_data = filepath_to_treenode_data(input_args->path);
  scanned_path *spbuf;
  add_rbtree(tree, &pt_data);
  spbuf = pt_data->data.file;
  // spbuf->status |= SCANNED;
  if(spbuf->head == NULL){
    spbuf->head = fsbuf;
    spbuf->tail = fsbuf;
  }else{
    spbuf->tail->next = fsbuf;
    spbuf->tail = fsbuf;
  }

  if(HAS_FILTER(fsconf.hasopt, FILTER)){
    int is_printable = 0;
    if(HASOPT(fsconf.hasopt, USER)){
      if(fsbuf->uid == fsconf.user)
        is_printable = 1;
    }
    if(HASOPT(fsconf.hasopt, GROUP)){
      if(fsbuf->gid == fsconf.group)
        is_printable = 1;
    }
    if(HASOPT(fsconf.hasopt, LENGTH)){
      if((fsbuf->size > fsconf.length_min)
      && (fsbuf->size < fsconf.length_max))
        is_printable = 1;
    }
    if(is_printable)
      printFstat(*fsbuf, input_args->path);
  }

  if(HASOPT(fsconf.hasopt, VERBOSE)){
	   printFstat(*fsbuf, input_args->path);
  }

  if(HASOPT(fsconf.hasopt, STAT)){
    update_stats_size(fsbuf->size);
    update_stats_type(fsbuf->mode);
  }

  if(HASOPT(fsconf.hasopt, REPORT)){
    update_report(fsbuf->size);
  }

  if(HASOPT(input_args->options, RECURSIVE)){
    if(S_ISDIR(fsbuf->mode)){
      if(HASOPT(input_args->options, FOLLOW_LINK)){
        treenode_data *inode_data = inode_to_treenode_data(stbuf.st_ino);
        //if tree doesn't contain this path
        if(get_data_rbtree(inode_tree, inode_data) == NULL){
          add_rbtree(inode_tree, &inode_data);
          dirwalk(input_args, filestat);
        }else{
          free(inode_data);
        }
      }else{
        dirwalk(input_args, filestat);
      }
    }
  }
  // if((HASOPT(input_args->options, RECURSIVE))
  // && (S_ISDIR(fsbuf->mode))
  // && !(HASOPT(spbuf->status, RECURSED))){
  //   dirwalk(input_args, filestat);
  // }
}

void dirwalk(input_file_argument *dir, void (*fcn)(input_file_argument *))
{
  // input_file_argument *file_arg;
  char *dirpath;
  char *filepath;
  struct dirent *dp;
  DIR *dfd;

  dirpath = dir->path;

  if((dfd = opendir(dir->path)) == NULL){
    fprintf(stderr, "dirwalk: can't open %s\n", dir->path);
    return;
  }

  while((dp = readdir(dfd)) != NULL){
    // printf("dirwalk: reading %s\n", dp->d_name);
    //ignore itself and its parent
    if(strcmp(dp->d_name, ".") == 0
    || strcmp(dp->d_name, "..") == 0)
      continue;
    if((filepath = malloc(sizeof(char) * (strlen(dirpath) + strlen(dp->d_name) + 2))) == NULL){
      fprintf(stderr, "%s", MALLOC_ERROR);
      exit(1);
    }
    sprintf(filepath, "%s/%s", dirpath, dp->d_name);
    // free(dir->path);
    dir->path=filepath;
    (*fcn)(dir);
  }
  closedir(dfd);
}

file_info statcpy(struct stat *stbuf)
{
  file_info fsbuf;

  time_t rawtime;
  time(&rawtime);

  fsbuf.date = rawtime;
  fsbuf.size = stbuf->st_size;
  fsbuf.mode = stbuf->st_mode;
  fsbuf.atime = stbuf->st_atime;
  fsbuf.ctime = stbuf->st_ctime;
  fsbuf.mtime = stbuf->st_mtime;
  fsbuf.nlink = stbuf->st_nlink;
  fsbuf.uid = stbuf->st_uid;
  fsbuf.gid = stbuf->st_gid;
  fsbuf.next = NULL;

  return fsbuf;
}
