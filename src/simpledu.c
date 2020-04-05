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
  info.objects= malloc(sizeof(Object)*32); //allocating memory for 32 objects
  info.index=0;
  info.max_size=32; //this value may be incremented if needed

  if(listThings(".",&info, &duflags)){
    printf("Error listing\n");
    regExit(1);
  }

  list_reg_files(&duflags,&info);
  free(info.objects);

  regExit(0);
}
