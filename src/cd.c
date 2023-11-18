#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "cd.h"

int cd(int argc, char **argv){
    if(argc < 2){
        fprintf(stderr, "Usage:%s <dir>\n", argv[0]);
        // exit(1);
    }
    if(chdir(argv[1]) < 0){
        perror("chdir()");
        // exit(1);
    }
    return 0;
}