#include "filestatConf.h"


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
      printf("%d ", optind);
      switch (nextOpt) {
        case 'v':
          fsconf[0]->hasopt |= VERBOSE;
          printf("opt: verbose\n");
          break;
        case 's':
          fsconf[0]->hasopt |= STAT;
          printf("opt: scan\n");
          break;
        case 'r':
          fsconf[0]->hasopt |= REPORT;
          printf("opt: report\n");
          break;
        case 'h':
          fsconf[0]->hasopt |= HISTORY;
          printf("opt: history = %s\n", optarg);
          break;
        case 'u':
          fsconf[0]->hasopt |= USER;
          printf("opt: user = %s\n", optarg);
          break;
        case 'g':
          fsconf[0]->hasopt |= GROUP;
          printf("opt: group = %s\n", optarg);
          break;
        case 'l':
          fsconf[0]->hasopt |= LENGTH;
          printf("opt: length = %s\n", optarg);
          break;
        case 'n':
          fsconf[0]->hasopt |= NOSCAN;
          printf("opt: noscan\n");
          break;
        default:
          printf("Invalid option\n");
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
