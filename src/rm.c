#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "rm.h"

int rm(int argc, char **argv) {
    int option = 0;
    char *path;
    char cur_path[1024];

    getcwd(cur_path, sizeof(cur_path));

    if(argc < 2) {
        fprintf(stderr, "Usage...\n");
        return 1;
    }

    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            if(strcmp(argv[i], "-r") == 0) {
                // printf("option = -r\n");
                option = 1;
            }
            else {
                fprintf(stderr, "Invalid option\n");
                return 1;
            }
        }
        else {
            path = argv[i];
        }
    }

    if(option == 0) {
        struct stat statres;
        if(stat(path, &statres) < 0) {
            perror("stat()");
            return 1;
        }
        if(S_ISDIR(statres.st_mode)) {
            DIR *dir = opendir(path);
            int contains = 0;
            struct dirent *entry;
            while((entry = readdir(dir)) != NULL) {
                if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
                contains = 1;
                break;
            }
            if(contains == 1){
                fprintf(stderr, "rm: cannot remove '%s': Directory not empty\n", path);
                printf(" %s\n", path);
                return 1;
            }
            else if(rmdir(path) < 0) {
                perror("rmdir()");
                return 1;
            }
        }
        
        else if(unlink(path) < 0) {
            perror("unlink()");
            return 1;
        }
    }
    else {
        struct stat statres;
        if(stat(path, &statres) < 0) {
            perror("stat()");
            printf(" %s\n", path);
            return 1;
        }
        if(S_ISDIR(statres.st_mode)) {
            DIR *dir = opendir(path);
            struct dirent *entry;
            while((entry = readdir(dir)) != NULL) {
                if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                    continue;
                }
                chdir(path);
                rm(3, (char *[]){argv[0], "-r", entry->d_name});
                chdir(cur_path);
            }
            if(rmdir(path) < 0) {
                perror("rmdir()");
                printf(" %s\n", path);
                return 1;
            }
        }    
        else if(unlink(path) < 0) {
            perror("unlink()");
            return 1;
        }
    }

    return 0;
}