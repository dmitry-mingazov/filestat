/*************************************************************************
                           filestat
            UNICAM - Scuola di scienze e tecnologie
                   Facolta' di informatica

            Corso di Sistemi Operativi Laboratorio

  Copyright (C) 2019  Dmitry Mingazov, Beatrice Pucci Sisti

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.                                *
*************************************************************************/
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
  stats.min_size = pow(2, (sizeof(stats.min_size) * 8) -2);
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
  long int files_tot = stats.nfiles + stats.nlink + stats.ndirs;
  if(files_tot == 0){
    stats.min_size = 0;
    return &stats;
  }
  stats.avg_size = stats.tot_size / files_tot;
  return &stats;
}

static clock_t start, end;
static program_report report;

void init_report(void)
{
  start = clock();
  report.nfiles = 0;
  report.max_size = 0;
  report.execution_time = 0;
}

void update_report(off_t size)
{
  report.nfiles++;
  if(report.max_size < size){
    report.max_size = size;
  }
}

program_report *end_report(void)
{
  end = clock();
  report.execution_time = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000;
  return &report;
}
