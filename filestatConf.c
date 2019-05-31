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
          printf("opt: verbose\n");
          break;
        case 's':
          printf("opt: scan\n");
          break;
        case 'r':
          printf("opt: report\n");
          break;
        case 'h':
          printf("opt: history = %s\n", optarg);
          break;
        case 'u':
          printf("opt: user = %s\n", optarg);
          break;
        case 'g':
          printf("opt: group = %s\n", optarg);
          break;
        case 'l':
          printf("opt: length = %s\n", optarg);
          break;
        case 'n':
          printf("opt: noscan\n");
          break;
        default:
          printf("Invalid option\n");
    }
  }
  printf("-----------\nOptIndex: %d\n", optind);
  if(optind < argc){
    // printf("Other ARGV elements\n");
    int i = 0;
    while(optind < argc && i < 2){
      if(i++ == 0){
        readInputFile(argv[optind++]);
      }
      else{
        printf("Output file: %s", argv[optind++]);
      }
    }
  }

  printf("Finished parsing options\n");
  return 0;
}
