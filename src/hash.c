#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hash_node_st{
    char *cmd;
    int (*handler)(int argc, char **argv);
    struct hash_node_st *next;
}HASH_NODE;

HASH_NODE *hash_table[26];

int hash_register(char *cmd, int (*handler)(int argc, char **argv)){
    int hash = cmd[0] - 'a';
    HASH_NODE *np;
    np = malloc(sizeof(*np));
    if(np == NULL){
        return -1;
    }
    np->cmd = cmd;
    np->handler = handler;
    np->next = hash_table[hash];
    hash_table[hash] = np;

    return 0;
}

HASH_NODE *hash_search(char *cmd){
    int hash = cmd[0] - 'a';
    HASH_NODE *np;
    for(np = hash_table[hash]; np != NULL; np = np->next){
        if(strcmp(np->cmd, cmd) == 0){
            return np;
        }
    }
    return NULL;
}