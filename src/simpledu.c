#include "simpledu.h"

int main(int argc, char const *argv[], char *envp[])
{
  flags duflags; initRegister(); initFlags(&duflags, envp);
  regCommand(argc,argv);

  if (setFlags(&duflags, argc, argv))
  {
    printf("Flag Error\n");
    regExit(1);
  }
  printFlags(&duflags);
  
  if (list_reg_files(&duflags))
  {
    printf("Read Dir Error\n");
    regExit(1);
  }

  regExit(0);
}
