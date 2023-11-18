#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mv.h"

int mv(int argc, char **argv) {
    char *src, *dest;
    char cur_path[1024];

    getcwd(cur_path, sizeof(cur_path));

    if(argc < 3) {
        fprintf(stderr, "Usage...\n");
        return 1;
    }

    src = argv[1];
    dest = argv[2];

    if(rename(src, dest) < 0) {
        perror("rename()");
        return 1;
    }

    return 0;
}