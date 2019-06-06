#include "filestat.h"

int main(int argc, char **argv)
{

  initFilestat(argc, argv, &fsconf);

  input_file_argument *input_args = fsconf.input_args;

  while(input_args != NULL){
    filestat(input_args);
    input_args = input_args->next;
  }

}


void filestat(input_file_argument *input_args)
{
  struct stat stbuf;
  file_info fsbuf;

  if((input_args->options & FOLLOW_LINK) == FOLLOW_LINK){
    if(stat(input_args->path, &stbuf) == -1){
      fprintf(stderr, "filestat: access denied at %s\n", input_args->path);
      return ;
    }
  }else{
    if(lstat(input_args->path, &stbuf) == -1){
      fprintf(stderr, "filestat: access denied at %s\n", input_args->path);
      return ;
    }
  }

  fsbuf = statcpy(&stbuf);


  if(((input_args->options & RECURSIVE) == RECURSIVE)
  && ((stbuf.st_mode & S_IFMT) == S_IFDIR )){
     dirwalk(input_args, filestat);
  }

  if((fsconf.hasopt & VERBOSE) == VERBOSE){

  }


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

  return fsbuf;
}
