#include "filestatConf.h"


void initFilestat(int argc, char *argv[], filestat_configuration *fsconf)
{
  getOptions(argc, argv, fsconf);



}


filestat_configuration getOptions(int argc, char *argv[], filestat_configuration *fsconf)
{

  if(argc < 2){
    printf("No arguments\n");
    return ;
  }
  fsconf->hasopt = 00;
  fsconf->output_file = DEFAULT_OUTPUT_FILE;

  fsconf->input_args = malloc(sizeof(input_file_argument));

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
          fsconf->hasopt |= VERBOSE;
          printf("opt: verbose\n");
          break;
        case 's':
          fsconf->hasopt |= STAT;
          printf("opt: scan\n");
          break;
        case 'r':
          fsconf->hasopt |= REPORT;
          printf("opt: report\n");
          break;
        case 'h':
          fsconf->hasopt |= HISTORY;
          printf("opt: history = %s\n", optarg);
          break;
        case 'u':
          fsconf->hasopt |= USER;
          printf("opt: user = %s\n", optarg);
          break;
        case 'g':
          fsconf->hasopt |= GROUP;
          printf("opt: group = %s\n", optarg);
          break;
        case 'l':
          fsconf->hasopt |= LENGTH;
          printf("opt: length = %s\n", optarg);
          break;
        case 'n':
          fsconf->hasopt |= NOSCAN;
          printf("opt: noscan\n");
          break;
        default:
          printf("Invalid option\n");
    }
  }
  printf("-------------");
  // printf("-----------\nOptIndex: %d\n", optind);
  if(optind < argc){
    // printf("Other ARGV elements\n");
    int i = 0;
    while(optind < argc && i < 2){
      if(i++ == 0){
        readInputFile(argv[optind++], fsconf->input_args);
      }
      else{
        fsconf->output_file =argv[optind++];
      }
    }
  }
  else{
    readInputFile(DEFAULT_INPUT_FILE, fsconf->input_args);
  }
  printf("Output file: %s\n", fsconf->output_file);
  printf("----------------\n");
  printf("SIZEOF hasopt: %ld\n", sizeof(fsconf->hasopt));
  printf("hasopt in:\n");
  char c = fsconf->hasopt;
  printf("octal: %o\nhex: %x\ndecimal: %d\n", c, c, c);

  // printf("Finished parsing options\n");
  return *fsconf;
}
