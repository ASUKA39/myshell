#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "tree.h"

#define TREE_L 1

static void tree_dir(const char *path, int depth, int level, int flag){
    DIR *dir, *tmp;
    struct dirent *entry;
    struct stat statbuf;
    char cwd[1024];
    int count = 0;

    if(depth == 0){
        return;
    }

    if((dir = opendir(path)) == NULL){
        perror("opendir()111");
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
            if(entry->d_name[0] == '.' && flag == 0){
                continue;
            }
            else if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
                continue;
            }
            else{
                for(int i = 0; i < level - depth; i++){
                    printf("│   ");
                }
                if(count == 0){
                    printf("└── %s\n", entry->d_name);
                    tree_dir(entry->d_name, depth-1, level, flag);
                }
                else{
                    printf("├── %s\n", entry->d_name);
                    tree_dir(entry->d_name, depth-1, level, flag);
                }
            }
        }
        else{
            if(entry->d_name[0] == '.' && flag == 0){
                continue;
            }
            else if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
                continue;
            }
            else{
                for(int i = 0; i < level - depth; i++){
                    printf("│   ");
                }
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
    int flag = 0;

    if(argc < 1){
        fprintf(stderr, "Usage...\n");
    }
    else if(argc == 1){
        path = ".";
    }

    else if(argc >= 2){
        path = ".";
        int depth_idx = -1;
        for(int i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                for(int j = 1; j < strlen(argv[i]); j++){
                    opt = argv[i][j];
                    switch(opt){
                        case 'L':
                            if(i+1 >= argc || !atoi(argv[i+1])){
                                printf("Option L missing arg\n");
                                return 1;
                            }
                            max_depth = atoi(argv[i+1]);
                            depth_idx = i + 1;
                            printf("max_depth = %d\n", max_depth);
                            break;
                        case 'a':
                            flag = 1;
                            break;
                        default:
                            fprintf(stderr, "Usage...\n");
                            return 1;
                    }
                }
            }
            else if(i != depth_idx){
                path = argv[i];
            }
        }
    }
    else{
        fprintf(stderr, "Arg...\n");
        return 1;
    }

    if(path == NULL){
        fprintf(stderr, "Path...\n");
        return 1;
    }
    else{
        printf("%s\n", path);
        tree_dir(path, max_depth, max_depth, flag);
    }

    return 0;
}

