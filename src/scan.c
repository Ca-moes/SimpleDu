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
  printf("maxDepth: %d\n\n", flags->maxDepthValue);

  while ((dentry = readdir(dir)) != NULL) {
    stat(dentry->d_name, &stat_entry);
    
    // Ficheiros Regulares
    if (S_ISREG(stat_entry.st_mode)) {
      if(flags->all){
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
    }
    // Diretorios
    else if (S_ISDIR(stat_entry.st_mode) && flags->maxDepthValue == 0) {
      if (strcmp(dentry->d_name, "..") == 0) continue;

      else if (flags->bytes) {
        int folderSize = stat_entry.st_size;
        if (strcmp(dentry->d_name, ".")) printf("%-d\t./%-25s\n", folderSize, dentry->d_name);
        else printf("%-d\t%-25s\n", folderSize, dentry->d_name);
      }

      else {
        int folderSize = stat_entry.st_size;
        int numBlocks = folderSize / stat_entry.st_blksize;
        if (strcmp(dentry->d_name, ".")) printf("%-d\t./%-25s\n", numBlocks, dentry->d_name);
        else printf("%-d\t%-25s\n", numBlocks, dentry->d_name);
      }
    }
    else printf("--max-depth != 1\n");
  }
  return 0; 
}