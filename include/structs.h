#ifndef STRUCTS
#define STRUCTS

#include <sys/types.h>
#include <time.h>

#define HASOPT(OPTIONS, OPT) (OPTIONS & OPT) == OPT

#define T_INODE 'I'
#define T_SCANNED_PATH 'S'

#define MALLOC_ERROR "[Error]: Out of memory\n"

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

typedef struct s_program_stats {
  unsigned long int nfiles;
  nlink_t nlink;
  unsigned long int ndirs;
  off_t tot_size;
  off_t avg_size;
  off_t max_size;
  off_t min_size;
} program_stat;

typedef struct s_program_report {
  unsigned long int nfiles;
  off_t max_size;
  double execution_time;
} program_report;

typedef union u_treenode_data {
  scanned_path *file;
  ino_t inode;
}u_data;

typedef struct s_treenode_data {
  unsigned char type;
  union u_treenode_data data;
} treenode_data;

#endif
