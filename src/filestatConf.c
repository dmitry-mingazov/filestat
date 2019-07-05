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
#include "filestatConf.h"

static void parse_length_argument(char *arg, off_t *min, off_t *max);
static char *err_invalid_argument = "Passed invalid argument\n";

int getOptions(int argc, char *argv[], filestat_configuration *fsconf)
{
  fsconf->hasopt = 00;
  fsconf->output_file = DEFAULT_OUTPUT_FILE;

  if((fsconf->input_args = malloc(sizeof(input_file_argument))) == NULL){
    fprintf(stderr, "%s", MALLOC_ERROR);
    exit(1);
  }



  int nextOpt;
  int optIndex = 0;
  const char* short_opts = "vsrh:u:g:l:";
  const struct option long_opts[] = {
    {"verbose", no_argument, NULL, 'v'},
    {"stat", no_argument, NULL, 's'},
    {"report", no_argument, NULL, 'r'},
    {"history", required_argument, NULL, 'h'},
    {"user", required_argument, NULL, 'u'},
    {"group", required_argument, NULL, 'g'},
    {"length", required_argument, NULL, 'l'},
    {"noscan", no_argument, NULL, 'n'},
    {NULL, 0, NULL, 0}
  };

  while((nextOpt = getopt_long(argc, argv, short_opts, long_opts, &optIndex)) != -1) {
      switch (nextOpt) {
        case 'v':
          fsconf->hasopt |= VERBOSE;
          break;
        case 's':
          fsconf->hasopt |= STAT;
          break;
        case 'r':
          fsconf->hasopt |= REPORT;
          break;
        case 'h':
          fsconf->hasopt |= HISTORY;
          fsconf->history = optarg;
          break;
        case 'u':
          fsconf->hasopt |= USER;
          if(sscanf(optarg, "%d", &fsconf->user) < 1){
            fprintf(stderr, "%s", err_invalid_argument);
            exit(1);
          }
          break;
        case 'g':
          fsconf->hasopt |= GROUP;
          if(sscanf(optarg, "%d", &fsconf->group) < 1){
            fprintf(stderr, "%s", err_invalid_argument);
            exit(1);
          }
          break;
        case 'l':
          fsconf->hasopt |= LENGTH;
          parse_length_argument(optarg, &fsconf->length_min, &fsconf->length_max);
          break;
        case 'n':
          fsconf->hasopt |= NOSCAN;
          break;
        default:
          printf("\n");
          exit(1);
    }
  }
  // printf("MIN: %ld MAX: %ld\n", fsconf[0]->length_min, fsconf[0]->length_max);
  // printf("-------------\n");
  // printf("-----------\nOptIndex: %d\n", optind);
  if(optind < argc){
    // printf("Other ARGV elements\n");
    int i = 0;
    while(optind < argc && i < 2){
      if(i++ == 0){
        fsconf->input_args = readInputFile(argv[optind++]);
      }
      else{
        fsconf->output_file =argv[optind++];
      }
    }
  }
  else{
    fsconf->input_args = readInputFile(DEFAULT_INPUT_FILE);
  }

  return 1;
}

void parse_length_argument(char *arg, off_t *min, off_t *max)
{
  char *str = arg;
  if(arg[0] == ':'){
    *min = 0;
    if(sscanf(arg, ":%ld", max) < 1){
      fprintf(stderr, "%s", err_invalid_argument);
      exit(1);
    }
    return;
  }
  if(arg[strlen(arg)-1] == ':'){
    *max =  pow(2, (sizeof(off_t) * 8) - 2);
    if(sscanf(arg, "%ld:", min) < 1){
      fprintf(stderr, "%s", err_invalid_argument);
      exit(1);
    }
    return;
  }

  while(*str != '\0' && *str != ':')
    str++;

  if(*str == ':'){
    if(sscanf(arg, "%ld:%ld", min, max) < 2){
      fprintf(stderr, "%s", err_invalid_argument);
      exit(1);
    }
  }
  else{
    *max =  pow(2, (sizeof(off_t) * 8) - 2);
    if(sscanf(arg, "%ld", min) < 1){
      fprintf(stderr, "%s", err_invalid_argument);
      exit(1);
    }
  }
}
