#!/bin/sh
touch hookMkdir.c
echo "#include <stdio.h> 
#include <sys/stat.h>
#include <sys/types.h>
int mkdir(const char *pathname, mode_t mode){printf(\"Hey, mkdir has been hooked!\\n\");return 0;}" >  hookMkdir.c
gcc -shared -o hookMkdir.so hookMkdir.c
export LD_PRELOAD="./hookMkdir.so"

