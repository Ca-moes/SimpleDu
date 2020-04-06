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

/* Read files in flags->dir
  Returns 0 OK; 1 otherwise
*/
void list_reg_files(flags *flags,char *path, struct stat stat_entry);

int listThings(char* directory_path, int depth, flags *dflags);

void SIGINT_handler(int signo);

void SIGINT_subscriber();

#endif /*SCAN_H*/