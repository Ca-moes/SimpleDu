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

      if (flags->bytes) {
        int fileSize = stat_entry.st_size;
        printf("%-d\t./%-25s\n", fileSize, dentry->d_name);
      }

      else {
        int fileSize = stat_entry.st_size;
        int numBlocks = fileSize / stat_entry.st_blksize;
        printf("%-d\t./%-25s\n", numBlocks, dentry->d_name);
      }
    }
    if (S_ISDIR(stat_entry.st_mode)) {
      if (strcmp(dentry->d_name, "..") == 0) continue;

      if (strcmp(dentry->d_name, ".") == 0) {
        int folderSize = stat_entry.st_size;
        printf("%-d\t%-25s\n", folderSize, dentry->d_name);
      }

      else if (flags->bytes) {
        int folderSize = stat_entry.st_size;
        printf("%-d\t./%-25s\n", folderSize, dentry->d_name);
      }

      else {
        int folderSize = stat_entry.st_size;
        int numBlocks = folderSize / stat_entry.st_blksize;
        printf("%-d\t./%-25s\n", numBlocks, dentry->d_name);
      }
    }
  }
  return 0; 
}