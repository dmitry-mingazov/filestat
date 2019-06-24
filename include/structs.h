#ifndef STRUCTS
#define STRUCTS

#include <sys/types.h>

typedef struct s_input_file_argument {
  char *path;
  unsigned char options;
  struct s_input_file_argument *next;
} input_file_argument;

typedef struct s_filestat_configuration {
  unsigned char hasopt;
  char *history;
  uid_t user;
  gid_t group;
  off_t length_min;
  off_t length_max;
  char *output_file;
  input_file_argument *input_args;
} filestat_configuration;

typedef struct s_file_info {
  time_t date;
  uid_t uid;
  gid_t gid;
  off_t size;
  mode_t mode;
  time_t atime;
  time_t ctime;
  time_t mtime;
  nlink_t nlink;
  struct s_file_info *next;
} file_info;

typedef struct s_path {
  char *path;
  file_info *head;
  file_info *tail;
} scanned_path;

#endif
