#include "reports.h"

static program_stat stats;

void init_stats(void)
{
  stats.nfiles = 0;
  stats.nlink = 0;
  stats.ndirs = 0;
  stats.tot_size = 0;
  stats.avg_size = 0;
  stats.max_size = 0;
  stats.min_size = pow(2, (sizeof(stats.min_size) * 8)) - 1;
}

void update_stats_type(mode_t mode)
{
  if(S_ISREG(mode)){
    stats.nfiles++;
  }else if(S_ISLNK(mode)){
    stats.nlink++;
  }else if(S_ISDIR(mode)){
    stats.ndirs++;
  }else{
    fprintf(stderr, "reports: file type not handled\n");
  }
}

void update_stats_size(off_t size)
{
  stats.tot_size += size;

  if(stats.max_size < size){
    stats.max_size = size;
  }
  if(stats.min_size > size){
    stats.min_size = size;
  }
}

program_stat *end_stats(void)
{
  stats.avg_size = stats.tot_size / (stats.nfiles + stats.nlink + stats.ndirs);
  return &stats;
}
