#include "simpledu.h"

int main(int argc, char const *argv[], char *envp[])
{
  flags duflags; initFlags(&duflags, envp); initRegister();
  regCommand(argc,argv);

  if (argc == 1)
    printf("Uso ./simpledu <dir> <flag1> <flag2> ..\n");
  else if (argc == 2){
    // ver página 1, ultimos pontos: "Por omissão, o comando du:"
    // -B=1; tamanho = 1024; -a=0; -b=0; -l=0; -L=0; --max-depth=0
    duflags.blockSize = 1;
    duflags.blockSizeValue = 1024; //default size
  }
  else{
    if (fillFlagsStruct(&duflags, argc, argv) != 0){
      printf("Flag Error\n");
      exit(1);
    }
  }
  
  printFlags(&duflags);
  
  regExit(0);
}
