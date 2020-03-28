#include "scan.h"

int list_reg_files(flags *flags) {
  
  DIR *dir;
  struct dirent *dentry;
  struct stat stat_entry;

  if ((dir = opendir(flags->dir)) == NULL) {
    perror(flags->dir);
    return 1;
  }
  chdir(flags->dir);

  printf("flags->bytes: %d\n", flags->bytes);

  while ((dentry = readdir(dir)) != NULL) {
    stat(dentry->d_name, &stat_entry);
    if (S_ISREG(stat_entry.st_mode)) {

      if (flags->bytes) {
        int fileSize = stat_entry.st_size;
        printf("%-d\t%-25s\n", fileSize, dentry->d_name);
      }

      else {
        int fileSize = stat_entry.st_size;
        int numBlocks = fileSize / stat_entry.st_blksize;
        printf("%-d\t%-25s\n", numBlocks, dentry->d_name);
      }
    }
  }
  return 0; 
}