#include "scan.h"

void list_reg_files(int dir, flags *flags,char *path, struct stat stat_entry) {
      if(flags->all){
          if (flags->bytes) {
          int fileSize = stat_entry.st_size;
          printf("%-d\t%-25s\n", fileSize, path);
        }

        else {
          int fileSize = stat_entry.st_size;
          int numBlocks = fileSize / flags->blockSizeValue;
          printf("%-d\t%-25s\n", numBlocks, path);
        }
      }
      else if (dir){
        if (flags->bytes) {
          int fileSize = stat_entry.st_size;
          printf("%-d\t%-25s\n", fileSize, path);
        }

        else {
          int fileSize = stat_entry.st_size;
          int numBlocks = fileSize / flags->blockSizeValue;
          printf("%-d\t%-25s\n", numBlocks, path);
        }
      }
}

int listThings(char* directory_path, int *depth, flags *dflags)
{
    DIR *dir;
    struct dirent *dentry;
    struct stat stat_entry;

    if ((dir = opendir(dflags->dir)) == NULL){
      perror(dflags->dir);
      return 1;
    }
    chdir(dflags->dir); //change to directory we are analysing
    
    while ((dentry = readdir(dir)) != NULL) {
      if (dflags->dereference)
        stat(dentry->d_name, &stat_entry);
      else
        lstat(dentry->d_name, &stat_entry);
      
      char new_path[1024]=""; //building new path of the file or directory
      strcat(new_path,directory_path);
      strcat(new_path,"/");
      strcat(new_path,dentry->d_name);
            
        // Ficheiros Regulares
        if (S_ISREG(stat_entry.st_mode)) {
            
            if (dflags->maxDepthValue>depth){
              list_reg_files(1,&dflags,new_path,stat_entry);
            }
        }
        // Diretorios
        else if (S_ISDIR(stat_entry.st_mode)) {
            dflags->dir=dentry->d_name; //update flag with path of directory we will analyse

            if ((strcmp(dentry->d_name, ".") == 0 || strcmp(dentry->d_name, "..") == 0)) //avoid infinite recursion
                continue;                                                                //we just want to make recursive calls to the directories inside "." not the direcotry itself
              int pid;
              pid= fork();  //saves child pids in array to wait for them later
              printf("here\n");
              if (pid==0){ //child process
                *depth+=1;
                printf("hereeeeeee\n");
                listThings(new_path,depth,dflags); //new process treats subdirectory making a recursive call
            
                regExit(0);

              }else if (pid<0){ //error on fork()
                printf("Error making fork\n");
                return 1;
              }
              else{ //parent process
              int status;
                waitpid(pid,&status,0);
              }

            if (dflags->maxDepthValue>=depth){
              list_reg_files(1,&dflags,new_path,stat_entry);
            }
        }
    }
    chdir("..");//go back to previous directory to continue listing things in there
    closedir(dir);
    return 0;
}
