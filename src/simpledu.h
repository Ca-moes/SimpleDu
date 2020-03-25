#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Flags
{
  // https://www.tutorialspoint.com/cprogramming/c_structures.htm
  // Bit Fields
  char **envip;
  unsigned int all : 1;  // -a OR --all
  unsigned int bytes : 1; // -b OR --bytes
  unsigned int blockSize : 1; // -B OR --block-size=SIZE
  unsigned short blockSizeValue : 16; // 16 bits tamanho maximo em bytes
  unsigned int countLinks : 1;  // -l OR --count-links
  unsigned int dereference : 1; // -L OR --dereference 
  unsigned int separateDirs : 1; // -S OR --separate-dirs
  unsigned int maxDepth : 1; // --max-depth=N
  int maxDepthValue;
};
typedef struct Flags flags;

void initFlags(flags *flags, char *envp[]);

// http://www.cplusplus.com/reference/cstring/strstr/
// http://www.cplusplus.com/reference/cstring/strcmp/
// Returns 0 if okay, 1 otherwise
int fillFlagsStruct(flags *flags, int argc, char const *argv[]);

void printFlags(flags *flags);