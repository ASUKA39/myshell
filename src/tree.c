#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "tree.h"

#define TREE_L 1

static void tree_dir(const char *path, int depth, int level){
    DIR *dir, *tmp;
    struct dirent *entry;
    struct stat statbuf;
    char cwd[1024];
    int count = 0;

    if(depth == 0){
        return;
    }

    if((dir = opendir(path)) == NULL){
        perror("opendir()");
        printf("%s\n", path);
        return;
    }

    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd()");
        return;
    }

    if(chdir(path) < 0){
        perror("chdir()");
        return;
    }

    if((tmp = opendir(".")) == NULL){
        perror("opendir(.)");
        return;
    }
    while((entry = readdir(tmp)) != NULL){
        if(lstat(entry->d_name, &statbuf) < 0){
            perror("lstat()");
            return;
        }
        count++;
    }

    while((entry = readdir(dir)) != NULL){
        if(lstat(entry->d_name, &statbuf) < 0){
            perror("lstat()");
            return;
        }
        count--;

        if(S_ISDIR(statbuf.st_mode)){
            if(entry->d_name[0] == '.'){
                continue;
            }
            else{
                if(count == 0){
                    printf("└── %s\n", entry->d_name);
                    tree_dir(entry->d_name, depth-1, level);
                }
                else{
                    printf("├── %s\n", entry->d_name);
                    tree_dir(entry->d_name, depth-1, level);
                }
            }
        }
        else{
            if(entry->d_name[0] == '.'){
                continue;
            }
            else{
                if(count == 0){
                    printf("└── %s\n", entry->d_name);
                }
                else{
                    printf("├── %s\n", entry->d_name);
                }
            }
        }
    }

    if(chdir(cwd) < 0){
        perror("chdir()");
        return;
    }

    if(closedir(tmp) < 0){
        perror("closedir(.)");
        return;
    }
    if(closedir(dir) < 0){
        perror("closedir()");
        return;
    }
    return;
}

int tree(int argc, char **argv){
    char *path = NULL;
    int opt;
    int max_depth = 1;

    if(argc < 1){
        fprintf(stderr, "Usage...\n");
    }
    else if(argc == 1){
        path = ".";
    }
    else if(argc == 2){
        path = argv[1];
    }
    else if(argc >= 3){
        path = ".";
        int depth_idx = 0;
        for(int i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                opt = argv[i][1];
                switch(opt){
                    case 'L':
                        max_depth = atoi(argv[i+1]);
                        depth_idx = i+1;
                        break;
                    default:
                        fprintf(stderr, "Usage...\n");
                        exit(1);
                }
            }
            else if(i != depth_idx){
                path = argv[i];
            }
        }
    }
    else{
        fprintf(stderr, "Arg...\n");
        exit(1);
    }

    if(path == NULL){
        fprintf(stderr, "Path...\n");
        exit(1);
    }
    else{
        printf("%s\n", path);
        tree_dir(path, max_depth, max_depth);
    }

    return 0;
}

