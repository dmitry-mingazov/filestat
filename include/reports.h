#ifndef REPORTS
#define REPORTS

#include <sys/stat.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "structs.h"


//initialize program_stat structure
void init_stats(void);
//update structure values based on type of file
void update_stats_type(mode_t mode);
//update structure values based on size of file
void update_stats_size(off_t size);
//calculate avg_size and return the program_stat
program_stat *end_stats(void);

void init_report(void);

void update_report(off_t size);

program_report *end_report(void);




#endif
