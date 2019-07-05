#ifndef PRINT
#define PRINT

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "structs.h"

void printFstat(file_info temp_file, char *filename);
void print_program_stats(program_stat *stat);
void print_program_report(program_report *report);
void print_output_file(scanned_path **pathlist, long int treesize);
void print_history_of_file(scanned_path *file);
#endif
