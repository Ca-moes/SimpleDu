#include "scan.h"

int list_reg_files(char *argv[]) {
  
  DIR *dir;
  struct dirent *dentry;
  struct stat stat_entry;

  if ((dir = opendir(argv[1])) == NULL) {
    perror(argv[1]);
    return 2;
  }
  chdir(argv[1]);

  while ((dentry = readdir(dir)) != NULL) {
    stat(dentry->d_name, &stat_entry);
    if (S_ISREG(stat_entry.st_mode)) {
      printf("%-8d\t%25s\n", (int)stat_entry.st_size, dentry->d_name);
    }
  }
  return 0; 
}