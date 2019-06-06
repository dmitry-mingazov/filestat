#include "utils.h"

char *parse_mode(mode_t mode)
{
  char permissions[10] = "rwxrwxrwx";

  return permissions;
  
  char no_permission = '-';

  if((mode & S_IRUSR) != S_IRUSR){
    permissions[0] = no_permission;
  }
  if((mode & S_IWUSR) != S_IWUSR){
    permissions[1] = no_permission;
  }


}
