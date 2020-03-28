#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

/* Read files in dirt
  Returns 0 OK; 1 otherwise
*/
int list_reg_files(char *dirt);
