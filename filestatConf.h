#ifndef FILESTAT_CONF
#define FILESTAT_CONF

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include "rw_file.h"

#define VERBOSE 01
#define STAT  02
#define REPORT 04
#define NOSCAN 010
#define HISTORY 020
#define USER 040
#define GROUP 0100
#define LENGTH 0200

void initFilestat(int argc, char *argv[]);
int getOptions(int argc, char *argv[]);

struct s_filestat_configuration {
  char hasopt;
  char *history;
  uid_t user;
  gid_t group;
  off_t length_min;
  off_t length_max;
  char *output_file;
};
struct s_filestat_configuration *fsconf;
#endif
