#include <stdio.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>

#include <dirent.h>
int mkdir(const char *pathname, mode_t mode){printf("Hey, mkdir has been hooked! If you want to unhook it, run unhook.sh by bash,  and restart the shell ");return 0;} 
