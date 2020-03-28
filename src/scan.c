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

  while ((dentry = readdir(dir)) != NULL) {
    stat(dentry->d_name, &stat_entry);
    if (S_ISREG(stat_entry.st_mode)) {
      printf("%-8d\t%25s\n", (int)stat_entry.st_blksize, dentry->d_name);
    }
  }
  return 0; 
}