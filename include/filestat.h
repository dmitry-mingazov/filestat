#ifndef FILESTAT
#define FILESTAT

#include "filestatConf.h"
#include <sys/stat.h>
#include <time.h>

typedef struct s_file_info {
  time_t data;
  off_t size;
  mode_t mode;
  time_t atime;
  time_t mtime;
  time_t ctime;
  nlink_t nlink;
  uid_t uid;
  gid_t gid;
} file_info;



static filestat_configuration fsconf;

void filestat(input_file_argument *input_args);
file_info statcpy(struct stat *stbuf);

#endif
