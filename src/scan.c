#include "scan.h"

int list_reg_files(flags *flags, char *path, struct stat stat_entry)
{
  int fileSize = stat_entry.st_size;

  if (flags->bytes)
    printf("%-d\t./%-25s\n", fileSize, path);
  else
  {
    int numBlocks = fileSize / flags->blockSizeValue;
    printf("%-d\t%-25s\n", numBlocks, path);
  }

  return 0;
}

int symlnkBS(char *path, const char *SLname, flags *flags)
{
  char pathcpy[128];
  char buf[50];

  strcpy(pathcpy, path);
  ssize_t len = readlink(SLname, buf, sizeof(buf) - 1);
  if (len == -1)
  {
    return 1;
  }
  else
  {
    char newpath[len];
    struct stat stats;

    buf[len] = '\0';
    strcpy(newpath, buf);
    stat(newpath, &stats);
    if (S_ISDIR(stats.st_mode))
    {
      listThingsSB(flags, newpath, stats, pathcpy);
    }
    else if (S_ISREG(stats.st_mode))
    {
      // printa path + /FILE
      int fileSize = stats.st_size;
      char *pch = strrchr(newpath, '/');
      strcat(pathcpy, pch);

      if (flags->bytes)
        printf("%-d\t./%-25s\n", fileSize, pathcpy);
      else
      {
        int numBlocks = fileSize / flags->blockSizeValue;
        printf("%-d\t%-25s\n", numBlocks, pathcpy);
      }

      return 0;
    }
  }
  return 0;
}

int listThingsSB(flags *flags, char *path, struct stat stat_entry, char *printpath)
{
  //lista diretório em path com o prefixo printpath
  struct dirent *dentry;
  char printpathvar[128];
  char pathvar[128];

  
  strcat(printpath, "/");
  DIR *dir = opendir(path);
  while ((dentry = readdir(dir)) != NULL)
  {
    strcpy(printpathvar, printpath);
    strcpy(pathvar, path);
    strcat(pathvar, "/");
    strcat(pathvar, dentry->d_name);
    stat(pathvar, &stat_entry);
    int fileSize = stat_entry.st_size;

    if ((strcmp(dentry->d_name, ".") == 0 || strcmp(dentry->d_name, "..") == 0)) //avoid infinite recursion
        continue;                                                                  //we just want to make recursive calls to the directories inside "." not the direcotry itself
    
    strcat(printpathvar, dentry->d_name);

    if (flags->bytes)
      printf("%-d\t./%-25s\n", fileSize, printpathvar);
    else
    {
      int numBlocks = fileSize / flags->blockSizeValue;
      printf("%-d\t%-25s\n", numBlocks, printpathvar);
    }
    
  }
  return 0;
}

int listThings(char *directory_path, flags *dflags)
{
  DIR *dir;
  struct dirent *dentry;
  struct stat stat_entry, checksl;
  dflags->maxDepthValue -= 1; //decrements depth value when enters directory

  if ((dir = opendir(dflags->dir)) == NULL)
  {
    perror(dflags->dir);
    return 1;
  }
  chdir(dflags->dir); //change to directory we are analysing

  while ((dentry = readdir(dir)) != NULL)
  {
    stat(dentry->d_name, &stat_entry);
    lstat(dentry->d_name, &checksl);

    char new_path[1024] = ""; //building new path of the file or directory
    strcat(new_path, directory_path);
    strcat(new_path, "/");
    strcat(new_path, dentry->d_name);

    // Links Simbolicos
    if (S_ISLNK(checksl.st_mode))
    {
      if (dflags->all)
      {
        symlnkBS(new_path, dentry->d_name, dflags);
      }
    }
    // Ficheiros Regulares
    if (S_ISREG(stat_entry.st_mode))
    {
      if (dflags->all)
        list_reg_files(dflags, new_path, stat_entry); //listing files
    }
    // Diretorios
    else if (S_ISDIR(stat_entry.st_mode))
    {
      dflags->dir = dentry->d_name; //update flag with path of directory we will analyse

      if ((strcmp(dentry->d_name, ".") == 0 || strcmp(dentry->d_name, "..") == 0)) //avoid infinite recursion
        continue;                                                                  //we just want to make recursive calls to the directories inside "." not the direcotry itself
      if (dflags->maxDepthValue > 0)
      { //only goes to subdirectory(create fork) if depth>0
        pid_t pids[1024];
        int pid_n = 0;
        pids[pid_n] = regFork(dflags); //saves child pids in array to wait for them later

        if (pids[pid_n] == 0)
        {
          listThings(new_path, dflags); //new process treats subdirectory making a recursive call
          regExit(0);
        }
        else if (pids[pid_n] < 0)
        { //error on fork()
          printf("Error making fork\n");
          return 1;
        }
        else
        {
          int status;

          for (int i = 0; i <= pid_n; i++)
          {
            waitpid(pids[i], &status, 0); //waiting for each child to terminate
          }
          pid_n++;
        }
      }
      list_reg_files(dflags, new_path, stat_entry); //listing directories
    }
  }
  chdir(".."); //go back to previous directory to continue listing things in there
  closedir(dir);
  dflags->maxDepthValue += 1; //increases depth value leaving directory
  return 0;
}
