#include "scan.h"

void list_reg_files(flags *dflags,char *path, struct stat stat_entry){
  if (dflags->bytes) {
      printf("%-ld\t%-25s\n", stat_entry.st_size, path);
  }

  else {
    int numBlocks = stat_entry.st_blocks*512.0 / dflags->blockSizeValue;
    printf("%-d\t%-25s\n", numBlocks, path);
  }
  return 0;
}


int listThings(char* directory_path, int depth, flags *dflags)
{
    DIR *dir;
    struct dirent *dentry;
    struct stat stat_entry;
    long int size=0;
    long int RecSubdirSize=0;

    if ((dir = opendir(dflags->dir)) == NULL){
      perror(dflags->dir);
      return -1;
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
        if (S_ISREG(stat_entry.st_mode) || S_ISLNK(stat_entry.st_mode)) { //we want to list regular files and symbolic links in the same way
            if (dflags->bytes) {
              size+=stat_entry.st_size;
              regEntry(stat_entry.st_size, new_path);
            }
            else {
              size += stat_entry.st_blocks*512.0 / dflags->blockSizeValue;
              regEntry(stat_entry.st_blocks*512.0 / dflags->blockSizeValue, new_path);
            }

            if (dflags->maxDepthValue> depth && dflags->all){ //printing files only if we're under maxDepth value
              list_reg_files(dflags,new_path,stat_entry);
            }
        }
        // Diretorios
        else if (S_ISDIR(stat_entry.st_mode)) {
            dflags->dir=dentry->d_name; //update flag with path of directory we will analyse
            if ((strcmp(dentry->d_name, ".") == 0 || strcmp(dentry->d_name, "..") == 0)) //avoid infinite recursion
                continue;                                                                //we just want to make recursive calls to the directories inside "." not the direcotry itself
              int pid, pd[2];
              pipe(pd);
              pid= fork();

              if (pid==0){ //child process
                regNewProcess(dflags);
                long int subdirSize=0;
                subdirSize+= listThings(new_path,depth+1,dflags); //new process treats subdirectory making a recursive call
                                                                  //returns size of whats inside directory
                close(pd[0]);
                regSendMessage(pd[1],&subdirSize,sizeof(long int));
                regExit(0);

              }else if (pid<0){ //error on fork()
                printf("Error making fork\n");
                return -1;
              }
              else{ //parent process
                waitpid(pid,NULL,0);
                
                close(pd[1]);
                regReceiveMessage(pd[0],&RecSubdirSize,sizeof(long int));

                if(dflags->bytes) RecSubdirSize+=4096; //an empty folder occupies 4096 bytes
                if(dflags->blockSize) RecSubdirSize+=stat_entry.st_blocks*512.0 / dflags->blockSizeValue; //one block corresponds to 512 bytes
                if (!dflags->separateDirs) size+=RecSubdirSize; //including subdirectory size
                
                regEntry(RecSubdirSize, new_path);

                if (dflags->maxDepthValue> depth) //printing subdirectories only if under maxDepthValue
                  printf("%-ld\t%-25s\n", RecSubdirSize, new_path);
              }
        }
    }
    if(depth==0){ //printing requested directory
        if(dflags->bytes) size+=4096;
        if(dflags->blockSize) size+=stat_entry.st_blocks*512.0 / dflags->blockSizeValue;
        printf("%-ld\t%-25s\n", size, directory_path);
    }

    chdir("..");//go back to previous directory to continue listing things in there
    closedir(dir);
    return size;
}
