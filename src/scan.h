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
}Object;

/* Read files in flags->dir
  Returns 0 OK; 1 otherwise
*/
void list_reg_files(int dir, flags *flags,char *path, struct stat stat_entry);

int listThings(char* directory_path, int *depth, flags *dflags);

#endif /*SCAN_H*/