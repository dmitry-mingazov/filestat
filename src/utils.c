#include "utils.h"

long int permbytes[10] = {S_IRUSR, S_IWUSR, S_IXUSR,
                         S_IRGRP, S_IWGRP, S_IXGRP,
                         S_IROTH, S_IWOTH, S_IXOTH
                        };

void *parse_mode(mode_t mode, char *permissions)
{
  sprintf(permissions, "%s", "rwxrwxrwx");

  // printf("parse_mode: %o\n", mode);


  char no_permission = '-';

  for(int i = 0; i < 9; i++){
    if((mode & permbytes[i]) != permbytes[i]){
      permissions[i] = no_permission;
    }
  }

  return "ops";

}
