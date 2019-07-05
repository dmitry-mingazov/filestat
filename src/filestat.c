#include "filestat.h"

static filestat_configuration fsconf;
static tree_descriptor *tree;
static tree_descriptor *inode_tree;

int main(int argc, char **argv)
{
  filestat_configuration *pfsconf = &fsconf;
  getOptions(argc, argv, &pfsconf);

  input_file_argument *input_args = fsconf.input_args;

  tree = init_rbtree();
  inode_tree = init_rbtree();

  if(HASOPT(fsconf.hasopt, STAT))
    init_stats();
  if(HASOPT(fsconf.hasopt, REPORT))
    init_report();

  readOutputFile(pfsconf->output_file, tree);

  long int treesize = 0;

  if(!(HASOPT(fsconf.hasopt, NOSCAN))){
    while(input_args != NULL){
      filestat(input_args);
      input_args = input_args->next;
    }
  }

  scanned_path **pathlist = inorder_visit(tree, &treesize);

  if(HASOPT(fsconf.hasopt, NOSCAN)){
    print_output_file(pathlist, treesize);
  }else{
    writeOutputFile(pfsconf->output_file, pathlist, treesize);
  }


  if(HASOPT(fsconf.hasopt, STAT))
    print_program_stats(end_stats());
  if(HASOPT(fsconf.hasopt, REPORT))
    print_program_report(end_report());

  //free della memoria (?)
}

void filestat(input_file_argument *input_args)
{
  struct stat stbuf;
  file_info *fsbuf = (file_info*) malloc(sizeof(file_info));

  if(HASOPT(input_args->options, FOLLOW_LINK)){
    if(stat(input_args->path, &stbuf) == -1){
      fprintf(stderr, "filestat: can't access to %s\n", input_args->path);
      return ;
    }
  }else{
    if(lstat(input_args->path, &stbuf) == -1){
      fprintf(stderr, "filestat: can't access to %s\n", input_args->path);
      return ;
    }
  }

  *fsbuf = statcpy(&stbuf);

  scanned_path sp_empty;
  sp_empty.path = input_args->path;
  sp_empty.status = SCANNED;
  sp_empty.head = NULL;
  sp_empty.tail = NULL;
  scanned_path *spbuf = (scanned_path*) malloc(sizeof(scanned_path));
  *spbuf = sp_empty;

  treenode_data *pt_data = (treenode_data*) malloc(sizeof(treenode_data));
  pt_data->type = T_SCANNED_PATH;
  union u_treenode_data data;
  data.file = spbuf;
  pt_data->data = data;

  pt_data = add_rbtree(tree, &pt_data);
  spbuf = pt_data->data.file;
  // spbuf->status |= SCANNED;
  if(spbuf->head == NULL){
    spbuf->head = fsbuf;
    spbuf->tail = fsbuf;
  }else{
    spbuf->tail->next = fsbuf;
    spbuf->tail = fsbuf;
  }

  /* printf("filestat: checked %s\n", input_args->path);
  // printf("---filestat: size %ld\n", fsbuf.size);
  // char perm[10];
  // parse_mode(fsbuf.mode, perm);
  // printf("---filestat: permissions %o\n", READABLE_PERMS(fsbuf.mode));
  // printf("---filestat: permissions %s\n", perm);*/
  if(HASOPT(fsconf.hasopt, VERBOSE)){
	   printFstat(*fsbuf, input_args->path);
  }

  if(HASOPT(fsconf.hasopt, STAT)){
    update_stats_size(fsbuf->size);
    update_stats_type(fsbuf->mode);
    /*printf("Number of files monitored: %d\n", );
    printf("Number of links: %d\n", );
    printf("Number of directories: %d\n", );
    printf("Tot files dimension: %ld\n", );
    printf("Avg file dimension: %ld\n", );
    printf("Max file dimension: %ld\n", );
    printf("Min file dimension: %ld\n", );*/
  }

  if(HASOPT(fsconf.hasopt, REPORT)){
    update_report(fsbuf->size);
  }

  if(HASOPT(input_args->options, RECURSIVE)){
    if(S_ISDIR(fsbuf->mode)){
      if(HASOPT(input_args->options, FOLLOW_LINK)){
        treenode_data *inode_data = (treenode_data*) malloc(sizeof(treenode_data));
        inode_data->type = T_INODE;
        inode_data->data.inode = stbuf.st_ino;
        if(!contains_rbtree(inode_tree, inode_data)){
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
  // printf("Dirwalk called and dirpath is: %s\n", dirpath);

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
    filepath = malloc(sizeof(char) * (strlen(dirpath) + strlen(dp->d_name) + 2));
    sprintf(filepath, "%s/%s", dirpath, dp->d_name);
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
