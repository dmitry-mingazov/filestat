#include "filestatConf.h"

static off_t parse_length_argument(char *arg);

void initFilestat(int argc, char *argv[], filestat_configuration *fsconf)
{
  getOptions(argc, argv, fsconf);



}


int getOptions(int argc, char *argv[], filestat_configuration **fsconf)
{

  // if(argc < 2){
  //   return 1;
  // }
  fsconf[0]->hasopt = 00;
  fsconf[0]->output_file = DEFAULT_OUTPUT_FILE;

  fsconf[0]->input_args = malloc(sizeof(input_file_argument));

  char *err_invalid_argument = "Passed invalid argument\n";

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

          break;
        case 'n':
          fsconf[0]->hasopt |= NOSCAN;
          break;
        default:
          printf("\n");
          exit(1);
    }
  }
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
  char c = fsconf[0]->hasopt;

  return 1;
}

// size_t parse_length_argument(char *arg)
// {
//
// }
