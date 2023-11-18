#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include "ls.h"
#include "pwd.h"
#include "cd.h"

#define LS_A 1
#define LS_L 2

int ls(int argc, char **argv){
    int options = 0;
    int pwd_idx = 0;

    if(argc < 1){
        fprintf(stderr, "Usage...\n");
    }
    else if(argc == 1){
        struct dirent *cur_dir;
        DIR *dir = opendir(".");
        while((cur_dir = readdir(dir)) != NULL){
            if(cur_dir->d_name[0] != '.')
                printf("%s  ", cur_dir->d_name);
        }
        closedir(dir);
    }
    else{
        for(int j = 1; j < argc; j++){
            printf("argv[%d] = %s\n", j, argv[j]);
            if(argv[j][0] == '-'){
                for(int i = 1; i < strlen(argv[j]); i++){
                    if(argv[j][i] == 'a'){
                        options |= LS_A;
                        // printf("A\n");
                    }
                    else if(argv[j][i] == 'l'){
                        options |= LS_L;
                        // printf("L\n");
                    }
                    else{
                        fprintf(stderr, "Invalid option %s\n", argv[j]);
                        return -1;
                    }
                }
            }
            else{
                if(!pwd_idx)
                    pwd_idx = j;
            }
        }

        struct dirent *cur_dir;
        DIR *dir;
        if(pwd_idx == 0)
            dir = opendir(".");
        else
            dir = opendir(argv[pwd_idx]);
        if(dir == NULL){
            perror("opendir()");
            return -1;
        }

        char pwd[1024];
        getcwd(pwd, 1024);
        chdir(argv[pwd_idx]);
        
        if(options & LS_L){
            while((cur_dir = readdir(dir)) != NULL){
                struct stat statbuf;
                if(cur_dir->d_name[0] != '.' || (options & LS_A)){
                    if(stat(cur_dir->d_name, &statbuf) < 0){
                        perror("stat()");
                        printf("%s\n", cur_dir->d_name);
                        return -1;
                    }
                    printf("%c%c%c%c%c  ", 
                        S_ISREG(statbuf.st_mode) ? '-' : 'd',
                        statbuf.st_mode & S_IRUSR ? 'r' : '-',
                        statbuf.st_mode & S_IWUSR ? 'w' : '-',
                        statbuf.st_mode & S_IXUSR ? 'x' : '-',
                        statbuf.st_mode & S_IXUSR ? 'x' : '-');
                    printf("%ld  ", statbuf.st_nlink);
                    printf("%ld  ", statbuf.st_size);
                    printf("%s  ", cur_dir->d_name);
                    printf("\n");
                }
            }
        }
        else{
            while((cur_dir = readdir(dir)) != NULL){
                if(cur_dir->d_name[0] != '.' || (options & LS_A))
                    printf("%s  ", cur_dir->d_name);
            }
        }
        closedir(dir);    

        chdir(pwd);
    }
    
    return 0;
}