#include "scan.h"

int listThings(char* directory_path, int depth, flags *dflags)
{
    DIR *dir;
    struct dirent *dentry;
    struct stat stat_entry;
    int size=0;
    int RecSubdirSize=0;

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
            }
            else {
              size += stat_entry.st_blocks*512.0 / dflags->blockSizeValue;
            }

            if (dflags->maxDepthValue> depth && dflags->all){ //printing files only if we're under maxDepth value
              if (dflags->bytes) {
                  printf("%-ld\t%-25s\n", stat_entry.st_size, new_path);
                }

              else {
                int numBlocks = stat_entry.st_blocks*512.0 / dflags->blockSizeValue;
                printf("%-d\t%-25s\n", numBlocks, new_path);
              }
            }
        }
        // Diretorios
        else if (S_ISDIR(stat_entry.st_mode)) {
            dflags->dir=dentry->d_name; //update flag with path of directory we will analyse
            if ((strcmp(dentry->d_name, ".") == 0 || strcmp(dentry->d_name, "..") == 0)) //avoid infinite recursion
                continue;                                                                //we just want to make recursive calls to the directories inside "." not the direcotry itself
              int pid;
              int pd[2];
              pipe(pd);
              pid= regFork(dflags);

              if (pid==0){ //child process
                int subdirSize=0;
                subdirSize+= listThings(new_path,depth+1,dflags); //new process treats subdirectory making a recursive call
                
                close(pd[0]);
                write(pd[1],&subdirSize,sizeof(long int));
                regExit(0);

              }else if (pid<0){ //error on fork()
                printf("Error making fork\n");
                return -1;
              }
              else{ //parent process
                waitpid(pid,NULL,0);
                
                close(pd[1]);
                read(pd[0],&RecSubdirSize,sizeof(long int));

                if(dflags->bytes) RecSubdirSize+=4096;
                if(dflags->blockSize) RecSubdirSize+=stat_entry.st_blocks*512.0 / dflags->blockSizeValue;
                if (!dflags->separateDirs) size+=RecSubdirSize;

                if (dflags->maxDepthValue> depth) //printing subdirectories only if under maxDepthValue
                  printf("%-d\t%-25s\n", RecSubdirSize, new_path);
              }
        }
    }
    if(depth==0){ //printing requested directory
        if(dflags->bytes) size+=4096;
        if(dflags->blockSize) size+=stat_entry.st_blocks*512.0 / dflags->blockSizeValue;
        printf("%-d\t%-25s\n", size, directory_path);
    }

    chdir("..");//go back to previous directory to continue listing things in there
    closedir(dir);
    return size;
}
