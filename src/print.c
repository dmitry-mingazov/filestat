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
#include "print.h"

static void print_file_info(file_info *info);
static void parse_mode(mode_t mode, char *permissions);

void printFstat(file_info temp_file, char* filename){
  printf(" %s\n\n", filename);
	print_file_info(&temp_file);
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

void print_output_file(scanned_path **pathlist, long int treesize)
{
  file_info *tmp_info;

  for(int i = 0; i < treesize; i++){
    printf("\n%s\n", pathlist[i]->path);
    tmp_info = pathlist[i]->head;
    while(tmp_info != NULL){
      print_file_info(tmp_info);
      tmp_info = tmp_info->next;
    }
    printf("-----------------------------\n");
  }
}

void print_history_of_file(scanned_path *file)
{
  file_info *tmp_info = file->head;
  printf("History of %s:\n", file->path);

  while(tmp_info != NULL){
    print_file_info(tmp_info);
    tmp_info = tmp_info->next;
  }
  printf("-----------------------------\n");
}

void print_file_info(file_info *info)
{
  printf("\nAcquired in date: %s",ctime(&info->date));
	printf("Size:            %ld \n",info->size);
	printf("Mode file:       %o \n",info->mode);
	printf("Access time:     %s",ctime(&info->atime));
	printf("Modified time:   %s",ctime(&info->mtime));
	printf("Change time:     %s",ctime(&info->ctime));
	printf("Number of links: %lu \n",info->nlink);
	printf("User ID:         %d \n",info->uid);
	printf("Group ID:        %d \n",info->gid);

  char perm[10];

	parse_mode(info->mode, perm);
	printf("Permissions:     %s\n", perm);
}

static long int permbytes[10] = {S_IRUSR, S_IWUSR, S_IXUSR,
                         S_IRGRP, S_IWGRP, S_IXGRP,
                         S_IROTH, S_IWOTH, S_IXOTH
                        };

void parse_mode(mode_t mode, char *permissions)
{
  sprintf(permissions, "%s", "rwxrwxrwx");

  char no_permission = '-';

  for(int i = 0; i < 9; i++){
    if((mode & permbytes[i]) != permbytes[i]){
      permissions[i] = no_permission;
    }
  }
}
