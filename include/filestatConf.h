#ifndef FILESTAT_CONF
#define FILESTAT_CONF

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "structs.h"
#include "rw_file.h"

#define VERBOSE 01
#define STAT  02
#define REPORT 04
#define NOSCAN 010
#define HISTORY 020
#define USER 040
#define GROUP 0100
#define LENGTH 0200

#define DEFAULT_INPUT_FILE "filestat.in"
#define DEFAULT_OUTPUT_FILE "filestat.db"

int getOptions(int argc, char *argv[], filestat_configuration **fsconf);

#endif
