#include <stdio.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>

#include <dirent.h>
DIR *opendir(const char *name){printf("Hey, opendir has been hooked! If you want to unhook it, run unhook.sh by bash,  and restart the shell  ");return 0;}
