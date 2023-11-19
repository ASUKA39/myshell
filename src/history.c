#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "history.h"

char *history_path = "/tmp/.history";

int history(int argc, char **argv) {
    char *linebuf = NULL;
    size_t linebuf_size = 0;
    FILE *fp = NULL;
    int i = 0;

    fp = fopen(history_path, "r");
    if(fp == NULL) {
        perror("fopen()");
        exit(1);
    }

    while(1) {
        if(getline(&linebuf, &linebuf_size, fp) < 0) {
            break;
        }
        i++;
        printf("%d %s", i, linebuf);
    }

    free(linebuf);
    return 0;
}

int append_history(char *linebuf) {
    FILE *fp = NULL;
    fp = fopen(history_path, "a");
    if(fp == NULL) {
        perror("fopen()");
        exit(1);
    }
    char timestr[32] = {0};
    time_t stamp;
    struct tm *tm;
    stamp = time(NULL);
    tm = localtime(&stamp);
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", tm);
    fprintf(fp, "%s\t", timestr);
    fprintf(fp, "%s", linebuf);
    fclose(fp);

    return 0;
}