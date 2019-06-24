#include "filestat.h"

static filestat_configuration fsconf;

int main(int argc, char **argv)
{
  filestat_configuration *pfsconf = &fsconf;
  getOptions(argc, argv, &pfsconf);

  // printf("HASOPT FROM MAIN: %d\n",fsconf.hasopt);

  input_file_argument *input_args = fsconf.input_args;

  // printf("main: root path -> %s\n", input_args->path);
  if(fsconf.input_args == NULL){
    printf("FSCONF.input_args NULL\n");
  }
  if(input_args == NULL){
    printf("No input file\n");
  }

  init_rbtree();

  readOutputFile(pfsconf->output_file);

  while(input_args != NULL){
    filestat(input_args);
    input_args = input_args->next;
  }
  inorder_visit();
  writeOutputFile(pfsconf->output_file);
}


void filestat(input_file_argument *input_args)
{
  struct stat stbuf;
  file_info *fsbuf = (file_info*) malloc(sizeof(file_info));

  if((input_args->options & FOLLOW_LINK) == FOLLOW_LINK){
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
  sp_empty.head = NULL;
  sp_empty.tail = NULL;
  scanned_path *spbuf = (scanned_path*) malloc(sizeof(scanned_path));
  *spbuf = sp_empty;
  spbuf = add_rbtree(spbuf);
  if(spbuf->head == NULL){
    spbuf->head = fsbuf;
    spbuf->tail = fsbuf;
  }else{
    spbuf->tail->next = fsbuf;
    spbuf->tail = fsbuf;
  }



  // printf("filestat: checked %s\n", input_args->path);
  // printf("---filestat: size %ld\n", fsbuf.size);
  // char perm[10];
  // parse_mode(fsbuf.mode, perm);
  // printf("---filestat: permissions %o\n", READABLE_PERMS(fsbuf.mode));
  // printf("---filestat: permissions %s\n", perm);
  if((fsconf.hasopt & VERBOSE) == VERBOSE){
	   printFstat(*fsbuf, input_args->path);
  }

  if(((input_args->options & RECURSIVE) == RECURSIVE)
  && (S_ISDIR(fsbuf->mode))){
    dirwalk(input_args, filestat);
  }
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
