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

  chdir(duflags.dir);

  if(listThings(".",0, &duflags)){
    printf("Error listing\n");
    regExit(1);
  }

  regExit(0);
}
