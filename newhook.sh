#/bin/bash

rm hook.c
touch hook.c

echo "#include <stdio.h> 

#include <sys/stat.h>

#include <sys/types.h>

#include <unistd.h>

#include <dirent.h>" >> hook.c

echo "Choose the items you need to hook:"

echo "Press 1 --- creating new folder:"

echo "Press 2 --- opening folder:"

echo "Press 3 --- renaming file: "

echo "Press 4 --- open file:"

read number

if test $number = "1"

then echo "int mkdir(const char *pathname, mode_t mode){printf(\"Hey, mkdir has been hooked!\\n If you want to unhook it,\\n run unhook.sh by bash, \\n and restart the shell \\n \");return 0;} " >> hook.c

echo "mkdir has been hooked now."

echo "Please restart the shell ,or input \"source .bashrc\""

elif test $number = "2"

then echo "DIR *opendir(const char *name){printf(\"Hey, opendir has been hooked!\\n If you want to unhook it,  \\n run unhook.sh by bash, \\n and restart the shell \\n \");return 0;}" >> hook.c

echo "ls has been hooked now."

echo "Please restart the shell ,or input \"source .bashrc\""

elif test $number = "3"

then echo "int rename(const char *oldpath, const char *newpath){printf(\"Hey, rename has been hooked!\\n If you want to unhook it,  \\n run unhook.sh by bash, \\n and restart the shell \\n \");return 0;}" >> hook.c

echo "mv has been hooked now."

echo "Please restart the shell ,or input \"source .bashrc\""

elif test $number = "4"

then echo "int open(const char *pathname, int flags){printf(\"Hey, open has been hooked!\\n If you want to unhook it, \\n and run unhook.sh by bash,\\n and restart the shell \\n \");return 0;}" >> hook.c

echo "open has been hooked now."

echo "Please restart the shell ,or input \"source .bashrc\""

else 

echo "error"

fi

#rm hookBack.sh

#touch hookBack.sh

#echo "echo \"export LD_PRELOAD = \" >> /home/peng.bashrc" >> hookBack.sh

#int _unlink( const char * pathname ) {printf(\"Hey, unlink has been hooked!\\n\");return 0;}

#size_t write(int fd, const void *buf, size_t count) {printf(\"Hey, write has been hooked!\\n\");return 0;}

#int remove(const char *pathname){printf(\"Hey, remove has been hooked!\\n\");return 0;}

#int _rmdir(const char *pathname){printf(\"Hey, rmdir has been hooked!\\n\");return 0;}

#struct dirent readdir(DIR *dir){printf(\"Hey, readdir has been hooked!\\n\");}

#int _chdir(const char *path){printf(\"Hey, opendir has been hooked!\\n\");return 0;}

gcc -shared -o hook.so hook.c

echo "export LD_PRELOAD=\"./hook.so\"" >> /home/peng/.bashrc

