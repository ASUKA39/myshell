#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>

#include "ls.h"
#include "pwd.h"
#include "cd.h"

#define LS_A 1
#define LS_L 2

static char *getperm(char *perm, struct stat fileStat);

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
            // printf("argv[%d] = %s\n", j, argv[j]);
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
                    char perm[11];
                    char datestring[256];
                    struct tm *tm = localtime(&statbuf.st_mtime);
                    strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);

                    printf("%10s ", getperm(perm, statbuf));
                    printf("%3ld ", statbuf.st_nlink);
                    printf("%-5s ", getpwuid(statbuf.st_uid)->pw_name);
                    printf("%-5s ", getgrgid(statbuf.st_gid)->gr_name);
                    printf("%6ld ", statbuf.st_size);
                    printf("%s ", datestring);
                    printf("%s", cur_dir->d_name);
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

char *getperm(char *perm, struct stat fileStat) {
	if(S_ISLNK(fileStat.st_mode)){
		perm[0] = 'l';
	}
	else if(S_ISDIR(fileStat.st_mode)){
		perm[0] = 'd';
	}
	else if(S_ISCHR(fileStat.st_mode)){
		perm[0] = 'c';
	}
	else if(S_ISSOCK(fileStat.st_mode)){
		perm[0] = 's';
	}
	else if(S_ISFIFO(fileStat.st_mode)){
		perm[0] = 'p';
	}
	else if(S_ISBLK(fileStat.st_mode)){
		perm[0] = 'b';
	}
	else {
		perm[0] = '-';
	}
	perm[1] = ((fileStat.st_mode & S_IRUSR) ? 'r' : '-');
	perm[2] = ((fileStat.st_mode & S_IWUSR) ? 'w' : '-');
	perm[3] = ((fileStat.st_mode & S_IXUSR) ? 'x' : '-');
	perm[4] = ((fileStat.st_mode & S_IRGRP) ? 'r' : '-');
	perm[5] = ((fileStat.st_mode & S_IWGRP) ? 'w' : '-');
	perm[6] = ((fileStat.st_mode & S_IXGRP) ? 'x' : '-');
	perm[7] = ((fileStat.st_mode & S_IROTH) ? 'r' : '-');
	perm[8] = ((fileStat.st_mode & S_IWOTH) ? 'w' : '-');
	perm[9] = ((fileStat.st_mode & S_IXOTH) ? 'x' : '-');

	if(fileStat.st_mode & S_ISUID){
		perm[3] = 's';
	}
	else if(fileStat.st_mode & S_IXUSR){
		perm[3] = 'x';
	}
	else {
		perm[3] = '-';
	}

	if(fileStat.st_mode & S_ISGID){
		perm[6] = 's';
	}
	else if(fileStat.st_mode & S_IXGRP){
		perm[6] = 'x';
	}
	else {
		perm[6] = '-';
	}

	if(fileStat.st_mode & S_ISVTX){
		perm[9] = 't';
	}
	else if(fileStat.st_mode & S_IXOTH){
		perm[9] = 'x';
	}
	else {
		perm[9] = '-';
	}
	perm[10] = 0;

	return perm;
}