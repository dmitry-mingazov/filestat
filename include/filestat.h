#ifndef FILESTAT
#define FILESTAT

#include "filestatConf.h"
#include <sys/stat.h>
#include <time.h>
#include <sys/dir.h>

#include "utils.h"
#include "rbtree.h"

typedef struct s_file_info {
  time_t date;
  off_t size;
  mode_t mode;
  time_t atime;
  time_t mtime;
  time_t ctime;
  nlink_t nlink;
  uid_t uid;
  gid_t gid;
} file_info;

#include "print.h"

static filestat_configuration fsconf;

void filestat(input_file_argument *input_args);
file_info statcpy(struct stat *stbuf);
void dirwalk(input_file_argument *dir, void (*fcn)(input_file_argument *));

#endif
