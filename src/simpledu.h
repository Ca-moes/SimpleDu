struct Flags
{
  // https://www.tutorialspoint.com/cprogramming/c_structures.htm
  // Bit Fields
  unsigned int all : 1;
  unsigned int bytes : 1;
  unsigned int blockSize : 1;
  unsigned short blockSizeValue : 16; // 16 bits tamanho maximo
  unsigned int countLinks : 1;
  unsigned int dereference : 1;
  unsigned int separateDirs : 1;
  unsigned int maxDepth : 1;
  int maxDepthValue;
};
typedef struct Flags flags;

void fillFlagsStruct(flags *flags, char const *argv[]);