#ifndef SCAN_H
#define SCAN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <wait.h>
#include <stdbool.h>
#include "flags.h"
#include "regfile.h"

typedef struct object{
  struct stat stat_entry;
  char path[100];
  bool dir;
  int size;
}Object;

typedef struct data{
    Object *objects; //array of objects
    int index; //index aka number of objects
    int max_size; //max_size of objects array -> updated if needed
    int all_size;
}Data;

/* Read files in flags->dir
  Returns 0 OK; 1 otherwise
*/
int list_reg_files(flags *flags,Data *info);

int listThings(char* directory_path,Data *info, flags *dflags);

int slashNumber(char *path);

#endif /*SCAN_H*/