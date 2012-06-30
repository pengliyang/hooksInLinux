#include <stdio.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>

#include <dirent.h>
int open(const char *pathname, int flags){printf("Hey, open has been hooked! If you want to unhook it,  and run unhook.sh by bash, and restart the shell  ");return 0;}
