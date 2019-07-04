#ifndef PRINT
#define PRINT

#include <stdio.h>
#include <time.h>
#include "structs.h"
#include "utils.h"

void printFstat(file_info temp_file, char *filename);
void print_program_stats(program_stat *stat);
void print_program_report(program_report *report);
#endif
