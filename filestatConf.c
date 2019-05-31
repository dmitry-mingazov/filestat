#include "filestatConf.h"


void initFilestat(int argc, char *argv[])
{
  getOptions(argc, argv);



}


int getOptions(int argc, char *argv[])
{

  if(argc<2){
    printf("No arguments\n");
    return 0;
  }
  fsconf = malloc(sizeof(struct s_filestat_configuration));
  fsconf->hasopt = 00;
  fsconf->output_file = DEFAULT_OUTPUT_FILE;

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
  printf("-------------\n");
  // printf("-----------\nOptIndex: %d\n", optind);
  if(optind < argc){
    // printf("Other ARGV elements\n");
    int i = 0;
    while(optind < argc && i < 2){
      if(i++ == 0){
        readInputFile(argv[optind++]);
      }
      else{
        fsconf->output_file =argv[optind++];
      }
    }
  }
  else{
    readInputFile(DEFAULT_INPUT_FILE);
  }
  printf("Output file: %s\n", fsconf->output_file);
  printf("----------------\n");
  printf("SIZEOF hasopt: %d\n", sizeof(fsconf->hasopt));
  printf("hasopt in:\n");
  char c = fsconf->hasopt;
  printf("octal: %o\nhex: %x\ndecimal: %d\n", c, c, c);

  printf("Finished parsing options\n");
  return 0;
}
