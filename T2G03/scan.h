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

#define WRITE 1
#define READ  0

struct sigaction oldaction;
// https://stackoverflow.com/questions/49184889/can-a-parent-send-signal-to-all-child-processes-in-the-same-exact-time
pid_t pgchldid;
int boole;

/* Read files in flags->dir
  Returns 0 OK; 1 otherwise
*/
void list_reg_files(flags *flags,char *path, struct stat stat_entry);

int listThings(char* directory_path, int depth, flags *dflags);

void SIGINT_handler(int signo);

void SIGINT_subscriber();

void no_SIGINT_handler();

#endif /*SCAN_H*/