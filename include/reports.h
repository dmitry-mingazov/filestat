#ifndef REPORTS
#define REPORTS

#include <sys/stat.h>
#include <math.h>
#include <stdio.h>
#include "structs.h"


//initialize fs_stats structure
void init_stats(void);
void update_stats_type(mode_t mode);
void update_stats_size(off_t size);
program_stat *end_stats(void);



#endif
