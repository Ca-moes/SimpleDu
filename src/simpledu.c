#include "simpledu.h"

int main(int argc, char const *argv[], char *envp[])
{
  flags duflags; initFlags(&duflags, envp); initRegister();
  regCommand(argc,argv);

  if (setFlags(&duflags, argc, argv))
  {
    printf("Flag Error\n");
    regExit(1);
  }
  
  if (list_reg_files(duflags.dir))
  {
    printf("Read Dir Error\n");
    regExit(1);
  }
  
  regExit(0);
}
