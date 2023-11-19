#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>
#include <sys/wait.h>
#include <time.h>

#include "main.h"

#define DELIMS " \t\n"

HASH_NODE *hash_table[26];

struct cmd_st {
    glob_t globres;
};

static void prompt(void) {
    char *pwd;
    char hostname[128];
    char username[128];

    pwd = getenv("PWD");
    if(pwd == NULL) {
        perror("getenv()");
        exit(1);
    }

    gethostname(hostname, 128);
    getlogin_r(username, 128);

    printf("┌──(%s@%s)-[%s]-\n", username, hostname, pwd);
    printf("└─$ ");
}

static void parse(char *line, struct cmd_st *res) {
    char *tok;
    int i = 0;
    while(1) {
        tok = strsep(&line, DELIMS);
        if(tok == NULL)
            break;
        if(tok[0] == '\0')
            continue;
        glob(tok, GLOB_NOCHECK|GLOB_APPEND*i, NULL, &res->globres);
        i = 1;
    }
}

static HASH_NODE *regist_func(void) {
    hash_register(hash_table, "cp", cp);
    hash_register(hash_table, "pwd", pwd);
    hash_register(hash_table, "cd", cd);
    hash_register(hash_table, "ls", ls);
    hash_register(hash_table, "tree", tree);
    hash_register(hash_table, "rm", rm);
    hash_register(hash_table, "mv", mv);
    hash_register(hash_table, "history", history);
    hash_register(hash_table, "ps", ps);
    
    return NULL;
}

int main(void) {
    char *linebuf = NULL;
    size_t linebuf_size = 0;
    struct cmd_st cmd;
    pid_t pid;

    regist_func();

    while(1) {
        prompt();
        if(getline(&linebuf, &linebuf_size, stdin) < 0) {
            break;
        }
        append_history(linebuf);

        parse(linebuf, &cmd);
        if(strcmp(cmd.globres.gl_pathv[0], "exit") == 0) {
            exit(0);
        }

        HASH_NODE *np;
        np = hash_search(hash_table, cmd.globres.gl_pathv[0]);
        if(np != NULL) {
            np->handler(cmd.globres.gl_pathc, cmd.globres.gl_pathv);
        }

        else {
            pid = fork();
            if(pid < 0) {
                perror("fork()");
                exit(1);
            }
            if(pid == 0) {
                execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
                perror("execvp()");
                exit(1);
            } else {
                wait(NULL);
            }
        }
        puts("");
    }
    exit(0);
}
