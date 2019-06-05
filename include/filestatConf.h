#ifndef FILESTAT_CONF
#define FILESTAT_CONF

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "rw_file.h"

#define VERBOSE 01
#define STAT  02
#define REPORT 04
#define NOSCAN 010
#define HISTORY 020
#define USER 040
#define GROUP 0100
#define LENGTH 0200

#define RECURSIVE 01
#define FOLLOW_LINK 02

#define DEFAULT_INPUT_FILE "filestat.in"
#define DEFAULT_OUTPUT_FILE "filestat.db"

typedef struct s_filestat_configuration {
  char hasopt;
  char *history;
  uid_t user;
  gid_t group;
  off_t length_min;
  off_t length_max;
  char *output_file;
} filestat_configuration;

typedef struct s_input_file_arguments {
  char *path;
  char options;
  struct s_input_file_arguments *next;
} input_file_arguments;

void initFilestat(int argc, char *argv[], filestat_configuration *fsconf);
int getOptions(int argc, char *argv[], filestat_configuration *fsconf);


#endif