#ifndef REPORTS
#define REPORTS

#include <sys/stat.h>

//initialize fs_stats structure
void init_stats(void);
void update_stats_type(mode_t mode);
void update_stats_size(off_t size);
void end_stats(void);



#endif
