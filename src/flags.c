#include "flags.h"

void initFlags(flags *flags, char *envp[]){

  int i=0;
  while (envp[i] != NULL)
    i++;
  flags->envip = malloc( sizeof(char*) * i-1 );
  i=0;
  while (envp[i] != NULL)
  {
    flags->envip[i] = envp[i];
    i++;
  }
  
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

int setFlags(flags *flags, int argc, char const *argv[]){
  if (argc == 1){
    printf("Use ./simpledu <dir> <flag1> <flag2> ..\n");
    return 1;
  }
  else if (argc == 2){
    // ver página 1, ultimos pontos: "Por omissão, o comando du:"
    // -B=1; tamanho = 1024; -a=0; -b=0; -l=0; -L=0; --max-depth=0
    flags->blockSize = 1;
    flags->blockSizeValue = 1024; //default size
  }
  else{
    if (fillFlagsStruct(flags, argc, argv) != 0){
      return 1;
    }
  }
  return 0;
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
        char tmp[18];
        strcpy(tmp, argv[i]);
        char *start = &tmp[13];
        char *end = &tmp[strlen(tmp)];
        char *substr = (char *)calloc(1, end - start + 1);
        memcpy(substr, start, end - start);
        flags->blockSizeValue = atoi(substr);
      }
    }
    else if (strstr(argv[i],"--max-depth=") != NULL)
    {
      if (flags->maxDepth == 1) return 1;
      else {
        flags->maxDepth = 1;
        // pôr valor de N em flags->maxdepthvalue
        char tmp[18];
        strcpy(tmp, argv[i]);
        char *start = &tmp[12];
        char *end = &tmp[strlen(tmp)];
        char *substr = (char *)calloc(1, end - start + 1);
        memcpy(substr, start, end - start);
        flags->maxDepthValue = atoi(substr);
      }
    }
    else if (argv[i][1] != '-'){
      flags->dir = malloc( sizeof(argv[i]));
      strcpy(flags->dir, argv[i]);
    }
    
  }
  return 0;
}

void printFlags(flags *flags){
  printf("Current Dir : %s\n", flags->dir);
  printf("-a OR --all : %d\n", flags->all);
  printf("-b OR --bytes : %d\n", flags->bytes);
  printf("-B OR --block-size=SIZE : %d\n", flags->blockSize);
  printf("SIZE : %d\n", flags->blockSizeValue);
  printf("-l OR --count-links : %d\n", flags->countLinks);
  printf("-L OR --dereference  : %d\n", flags->dereference);
  printf("-S OR --separate-dirs : %d\n", flags->separateDirs);
  printf("--max-depth=N : %d\n", flags->maxDepth);
  printf("N : %d\n", flags->maxDepthValue);

  // Enviroment Variables print
  /*int i=0;
  while (flags->envip[i] != NULL)
  {
    printf("%s\n", flags->envip[i]);
    i++;
  }*/
}
