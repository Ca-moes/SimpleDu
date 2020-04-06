#include "scan.h"

int slashNumber(char path[]){
  int i, count;
  for (i=0, count=0; path[i]!='\0'; i++){
    if (path[i] == '/')
      count++;
  }
  return count;
}

int list_reg_files(flags *flags,Data *info) {
  for (int i=0;i<info->index;i++){
    if (slashNumber(info->objects[i].path)<=flags->maxDepthValue){
      if(flags->all){
          if (flags->bytes) {
          int fileSize = info->objects[i].size;
          printf("%-d\t%-25s\n", fileSize, info->objects[i].path);
        }

        else {
          int fileSize = info->objects[i].size;
          int numBlocks = fileSize / flags->blockSizeValue;
          printf("%-d\t%-25s\n", numBlocks, info->objects[i].path);
        }
      }
      else if (info->objects[i].dir){
        if (flags->bytes) {
          int fileSize = info->objects[i].size;
          printf("%-d\t%-25s\n", fileSize, info->objects[i].path);
        }

        else {
          int fileSize = info->objects[i].size;
          int numBlocks = fileSize / flags->blockSizeValue;
          printf("%-d\t%-25s\n", numBlocks, info->objects[i].path);
        }
      }
    }
  }
  // . directory
  if (flags->bytes) {
    int fileSize = info->all_size;
    printf("%-d\t%-25s\n", fileSize, ".");
  }

  else {
    int fileSize = info->all_size;
    int numBlocks = fileSize / flags->blockSizeValue;
    printf("%-d\t%-25s\n", numBlocks, ".");
  }
  
  return 0; 
}

int listThings(char* directory_path, Data *data, flags *dflags)
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
            if (data->index == data->max_size) { //see if we can insert another element on objects array otherwise we duplicate the memory allocated
              data->objects = realloc(data->objects, 2 * data->max_size * sizeof(Object));
              data->max_size *= 2; //updates max size
            }
            
            strcpy(data->objects[data->index].path,new_path); //Adding file to array
            data->objects[data->index].stat_entry=stat_entry;
            data->objects[data->index].dir=false;
            data->objects[data->index].size=stat_entry.st_size;
            data->index+=1; //updates index
        }
        // Diretorios
        else if (S_ISDIR(stat_entry.st_mode)) {
            dflags->dir=dentry->d_name; //update flag with path of directory we will analyse

            if ((strcmp(dentry->d_name, ".") == 0 || strcmp(dentry->d_name, "..") == 0)) //avoid infinite recursion
                continue;                                                                //we just want to make recursive calls to the directories inside "." not the direcotry itself
              pid_t pids[1024];
              int pid_n=0, pd[2];
              pipe(pd);
              pids[pid_n] = regFork(dflags);  //saves child pids in array to wait for them later
              
              if (pids[pid_n]==0){ //child process
                listThings(new_path,data,dflags); //new process treats subdirectory making a recursive call
                
                close(pd[0]);
                write(pd[1],&data->index,sizeof(int)); //send index to father process (number of elements we have now)
                write(pd[1],data->objects,sizeof(Object)*(data->index)); //send the array of objects updated
                regExit(0);

              }else if (pids[pid_n]<0){ //error on fork()
                printf("Error making fork\n");
                return 1;
              }
              else{ //parent process
                int status;
                
                for (int i=0;i<=pid_n;i++){
                  waitpid(pids[i],&status,0); //waiting for each child to terminate
                }
                pid_n++;

                int index;
                close(pd[1]);
                read(pd[0],&index,sizeof(int)); //read the index sent by child (total number of elements)
                
                while(index > data->max_size){ //if the size of the array with the new elements is larger than max size we need to reallocate
                  data->objects = realloc(data->objects, 2 * data->max_size * sizeof(Object)); //while loop because reallocating once may not be enough (in case a directory having a lot of subdirectories and files)
                }
                
                read(pd[0], data->objects, sizeof(Object)*(index)); //replace parent array with the child array updated
                data->index=index; //updating parent index
                data->all_size+=data->objects[data->index].size;
              }
            if (data->index == data->max_size) { //checking again if we can insert another object on array
              data->objects = realloc(data->objects, 2 * data->max_size * sizeof(Object));
              data->max_size *= 2;
            }
            strcpy(data->objects[data->index].path,new_path); //adding directory to array
            data->objects[data->index].stat_entry=stat_entry;
            data->objects[data->index].dir=true;
            data->objects[data->index].size=data->objects[data->index].stat_entry.st_size;
            data->index+=1; //updates index
        }
    }
    chdir("..");//go back to previous directory to continue listing things in there
    closedir(dir);
    return 0;
}
