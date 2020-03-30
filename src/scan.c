#include "scan.h"

int list_reg_files(flags *flags,char *path, struct stat stat_entry) {

  if (flags->bytes) {
    int fileSize = stat_entry.st_size;
    printf("%-d\t./%-25s\n", fileSize, path);
  }

  else {
    int fileSize = stat_entry.st_size;
    int numBlocks = fileSize / stat_entry.st_blksize;
    printf("%-d\t%-25s\n", numBlocks, path);
  }
  return 0; 
}

void listdir(char* directory_path,flags *dflags)
{
    DIR *dir;
    struct dirent *dentry;
    struct stat stat_entry;

    if ((dir = opendir(dflags->dir)) == NULL){
      perror(dflags->dir);
      return;
    }
    chdir(dflags->dir); //change to directory we are analysing
    
    while ((dentry = readdir(dir)) != NULL) {
      stat(dentry->d_name, &stat_entry);

      char new_path[1024]=""; //building new path of the file or directory
      strcat(new_path,directory_path);
      strcat(new_path,"/");
      strcat(new_path,dentry->d_name);

        // Ficheiros Regulares
        if (S_ISREG(stat_entry.st_mode)) {
          if(dflags->all)
            list_reg_files(dflags,new_path,stat_entry); //listing files
        }
        // Diretorios
        else if (S_ISDIR(stat_entry.st_mode)) {
            dflags->dir=dentry->d_name; //update flag with path of directory we will analyse

            if ((strcmp(dentry->d_name, ".") == 0 || strcmp(dentry->d_name, "..") == 0)) //avoid infinite recursion
                continue;                                                                //we just want to make recursive calls to the directories inside "." that is our ../TestDir (first case)
            
            listdir(new_path,dflags);
            list_reg_files(dflags, new_path, stat_entry); //listing directories
        }
    }
    chdir("..");//go back to previous directory to continue listing things in there
    closedir(dir);
    return;
}