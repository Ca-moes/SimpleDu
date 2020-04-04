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

  Data info;
  info.objects= malloc(sizeof(Object)*32);
  info.index=0;
  info.max_size=32;

  if(listThings(".",&info, &duflags)){
    printf("Error listing\n");
    regExit(1);
  }

  list_reg_files(&duflags,&info);

  regExit(0);
}
