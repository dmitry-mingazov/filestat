#ifndef FILESTAT
#define FILESTAT

#include <sys/stat.h>
#include <time.h>
#include <sys/dir.h>
#include "structs.h"

#include "filestatConf.h"
#include "rbtree.h"
#include "print.h"
#include "rw_file.h"
#include "reports.h"

void filestat(input_file_argument *input_args);
file_info statcpy(struct stat *stbuf);
void dirwalk(input_file_argument *dir, void (*fcn)(input_file_argument *));

#endif
