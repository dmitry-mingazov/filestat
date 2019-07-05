#include "filestatConf.h"

static void parse_length_argument(char *arg, off_t *min, off_t *max);
static char *err_invalid_argument = "Passed invalid argument\n";

int getOptions(int argc, char *argv[], filestat_configuration **fsconf)
{

  // if(argc < 2){
  //   return 1;
  // }
  fsconf[0]->hasopt = 00;
  fsconf[0]->output_file = DEFAULT_OUTPUT_FILE;

  if((fsconf[0]->input_args = malloc(sizeof(input_file_argument))) == NULL){
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
          fsconf[0]->hasopt |= VERBOSE;
          break;
        case 's':
          fsconf[0]->hasopt |= STAT;
          break;
        case 'r':
          fsconf[0]->hasopt |= REPORT;
          break;
        case 'h':
          fsconf[0]->hasopt |= HISTORY;
          fsconf[0]->history = optarg;
          break;
        case 'u':
          fsconf[0]->hasopt |= USER;
          if(sscanf(optarg, "%d", &fsconf[0]->user) < 1){
            fprintf(stderr, "%s", err_invalid_argument);
            exit(1);
          }
          break;
        case 'g':
          fsconf[0]->hasopt |= GROUP;
          if(sscanf(optarg, "%d", &fsconf[0]->group) < 1){
            fprintf(stderr, "%s", err_invalid_argument);
            exit(1);
          }
          break;
        case 'l':
          fsconf[0]->hasopt |= LENGTH;
          parse_length_argument(optarg, &fsconf[0]->length_min, &fsconf[0]->length_max);
          break;
        case 'n':
          fsconf[0]->hasopt |= NOSCAN;
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
        fsconf[0]->input_args = readInputFile(argv[optind++]);
      }
      else{
        fsconf[0]->output_file =argv[optind++];
      }
    }
  }
  else{
    fsconf[0]->input_args = readInputFile(DEFAULT_INPUT_FILE);
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
