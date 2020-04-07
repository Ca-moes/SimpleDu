#include "simpledu.h"

int main(int argc, char const *argv[], char *envp[])
{
  SIGINT_subscriber();
  flags duflags; initRegister(); initFlags(&duflags, envp);
  regCommand(argc,argv);

  if (setFlags(&duflags, argc, argv))
  {
    printf("Flag Error\n");
    regExit(1);
  }
  //printFlags(&duflags);

  if(listThings(duflags.dir,0, &duflags)<0){
    printf("Error listing\n");
    regExit(1);
  }

  regExit(0);
}
