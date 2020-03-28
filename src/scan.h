#ifndef SCAN_H
#define SCAN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include "flags.h"

/* Read files in flags->dir
  Returns 0 OK; 1 otherwise
*/
int list_reg_files(flags *flags);

#endif /*SCAN_H*/