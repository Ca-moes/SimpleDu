#include "simpledu.h"

int main(int argc, char const *argv[], char *envp[])
{
  // Objetivo 1:
  // Receber, processar e guardar os
  //  argumentos da linha de comandos e as variáveis de ambiente;
  flags duflags; initFlags(&duflags);

  if (argc == 1)
    printf("Uso ./simpledu <dir> <flag1> <flag2> ..\n");
  else if (argc == 2)
  {
    // ver página 1, ultimos pontos: "Por omissão, o comando du:"
    // acrescentar estes pontos na struct flags
    duflags.blockSizeValue = 1024; //default size
  }
  else{
    if (fillFlagsStruct(&duflags, argc, argv) != 0){
      printf("Flag Error\n");
      exit(1);
    }
  }
  
  printFlags(&duflags);

  return 0;
}


void initFlags(flags *flags){
  flags->all = 0;
  flags->bytes = 0;
  flags->blockSize = 0;
  flags->blockSizeValue = 0;
  flags->countLinks = 0;
  flags->dereference = 0;
  flags->separateDirs = 0;
  flags->maxDepth = 0;
  flags->maxDepthValue = 0;
}

int fillFlagsStruct(flags *flags, int argc, char const *argv[])
{
  for (unsigned int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0)
    {
      if (flags->all == 1) return 1;
      else flags->all = 1;
    } 
    else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes") == 0)
    {
      if (flags->bytes == 1) return 1;
      else flags->bytes = 1;
    } 
    else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--count-links") == 0)
    {
      if (flags->countLinks == 1) return 1;
      else flags->countLinks = 1;
    } 
    else if (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--dereference") == 0)
    {
      if (flags->dereference == 1) return 1;
      else flags->dereference = 1;
    } 
    else if (strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--separate-dirs") == 0)
    {
      if (flags->separateDirs == 1) return 1;
      else flags->separateDirs = 1;
    } 
    else if (strcmp(argv[i], "-B") == 0 || strstr(argv[i],"--block-size=") != NULL)
    {
      if (flags->blockSize == 1) return 1;
      else {
        flags->blockSize = 1;
        // pôr valor de SIZE em flags->blockSizeValue
      }
    }
    else if (strstr(argv[i],"--max-depth=") != NULL)
    {
      if (flags->maxDepth == 1) return 1;
      else {
        flags->maxDepth = 1;
        // pôr valor de N em flags->maxdepthvalue
      }
    }
  }
  return 0;
}

void printFlags(flags *flags){
  printf("-a OR --all : %d\n", flags->all);
  printf("-b OR --bytes : %d\n", flags->bytes);
  printf("-B OR --block-size=SIZE : %d\n", flags->blockSize);
  printf("SIZE : %d\n", flags->blockSizeValue);
  printf("-l OR --count-links : %d\n", flags->countLinks);
  printf("-L OR --dereference  : %d\n", flags->dereference);
  printf("-S OR --separate-dirs : %d\n", flags->separateDirs);
  printf("--max-depth=N : %d\n", flags->maxDepth);
  printf("N : %d\n", flags->maxDepthValue);
}