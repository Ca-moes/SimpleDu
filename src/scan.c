#include "scan.h"

void list_reg_files(flags *dflags,char *path, struct stat stat_entry){
  if (dflags->bytes) {
      printf("%-ld\t%-25s\n", stat_entry.st_size, path);
  }

  else {
    int numBlocks = stat_entry.st_blocks*512.0 / dflags->blockSizeValue;
    printf("%-d\t%-25s\n", numBlocks, path);
  }
  return;
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
                close(pd[READ]);
                regSendMessage(pd[WRITE],&subdirSize,sizeof(long int));
                close(pd[WRITE]);
                regExit(0);

              }else if (pid<0){ //error on fork()
                printf("Error making fork\n");
                return -1;
              }
              else{ //parent process
                waitpid(pid,NULL,0);
                
                close(pd[WRITE]);
                regReceiveMessage(pd[READ],&RecSubdirSize,sizeof(long int));
                close(pd[READ]);

                if(dflags->bytes) RecSubdirSize+=stat_entry.st_size; //aparentemente isto varia n é smp o mesmo valor
                if(dflags->blockSize) RecSubdirSize+=stat_entry.st_blocks*512.0 / dflags->blockSizeValue; //one block corresponds to 512 bytes
                if (!dflags->separateDirs) size+=RecSubdirSize; //including subdirectory size
                
                regEntry(RecSubdirSize, new_path);

                if (dflags->maxDepthValue> depth) //printing subdirectories only if under maxDepthValue
                  printf("%-ld\t%-25s\n", RecSubdirSize, new_path);
              }
        }
    }
    if(depth==0){ //printing requested directory
        if(dflags->bytes) size+=stat_entry.st_size;
        if(dflags->blockSize) size+=stat_entry.st_blocks*512.0 / dflags->blockSizeValue;
        printf("%-ld\t%-25s\n", size, directory_path);
    }

    chdir("..");//go back to previous directory to continue listing things in there
    closedir(dir);
    return size;
}

void SIGINT_handler(int signo) {
  char quit;
  regSendSignal(getppid(), SIGSTOP);

  while (1) {
    printf("Are you sure you want to quit?\nY: Yes, I want to quit\nN: No, I want to continue\n");
    scanf("%s", &quit);

    if (quit == 'N' || quit == 'n') {
      regSendSignal(getppid(), SIGCONT);
      return;
    }
    else if (quit == 'Y' || quit == 'y') {
      regSendSignal(getppid(), SIGQUIT);
      regSendSignal(getpid(), SIGQUIT);
      return;
    }
    else continue;
  }
}

void SIGINT_subscriber() {
  struct sigaction action;
  action.sa_handler = SIGINT_handler;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  sigaction(SIGINT, &action, NULL);
}