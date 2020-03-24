#include <stdio.h>
#include "simpledu.h"

int main(int argc, char const *argv[], char *envp[])
{
  // Objetivo 1:
  // Receber, processar e guardar os
  //  argumentos da linha de comandos e as variáveis de ambiente;
  flags duflags;
  duflags.blockSizeValue = 1024; //default size

  printf("Declarou tudo, next -> funções\n");
  if (argc == 1)
    printf("Uso ./simpledu <dir> <flag1> <flag2> ..");
  else if (argc == 2)
  {
    // ver página 1, ultimos pontos: "Por omissão, o comando du:"
    // acrescentar estes pontos na struct flags
  }
  else
    fillFlagsStruct(&duflags, argv);

  return 0;
}

void fillFlagsStruct(flags *flags, char const *argv[])
{
  printf("all ok\n");
}