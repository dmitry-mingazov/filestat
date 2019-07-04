#include "print.h"

void printFstat(file_info temp_file, char* filename){
  printf(" %s\n\n", filename);
	printf("Acquired in date: %s \n",ctime(&temp_file.date));
	printf("Size:            %ld \n",temp_file.size);
	printf("Mode file:       %o \n",temp_file.mode);
	printf("Access time:     %s",ctime(&temp_file.atime));
	printf("Modified time:   %s",ctime(&temp_file.mtime));
	printf("Change time:     %s",ctime(&temp_file.ctime));
	printf("Number of links: %lu \n",temp_file.nlink);
	printf("User ID:         %d \n",temp_file.uid);
	printf("Group ID:        %d \n",temp_file.gid);

  char perm[10];

	parse_mode(temp_file.mode, perm);
	printf("Permissions:     %s\n", perm);
  printf("-----------------------------\n");
}

void print_program_stats(program_stat *stats)
{
  printf("\nStats:\n");
  printf("Number of files:\t%ld\n", stats->nfiles);
  printf("Number of links:\t%lu\n", stats->nlink);
  printf("Number of directories:\t%ld\n", stats->ndirs);
  printf("Total size:\t%ld\n", stats->tot_size);
  printf("Average size:\t%ld\n", stats->avg_size);
  printf("Maximum size:\t%ld\n", stats->max_size);
  printf("Minimum size:\t%ld\n", stats->min_size);
}

void print_program_report(program_report *report)
{
  printf("\nReport:\n");
  printf("Number of files:\t%ld\n", report->nfiles);
  printf("Maximum size:\t%ld\n", report->max_size);
  printf("Execution time:\t%f ms\n", report->execution_time);
}
