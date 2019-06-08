#ifndef UTILS
#define UTILS

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define READABLE_PERMS(mode) mode & 0777

void *parse_mode(mode_t mode, char *permissions);

#endif
