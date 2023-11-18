#ifndef HASH_H__
#define HASH_H__

typedef struct hash_node_st{
    char *cmd;
    int (*handler)(int argc, char **argv);
    struct hash_node_st *next;
}HASH_NODE;

int hash_register(HASH_NODE *hash_table[], const char *name, int (*func)(int, char **));
HASH_NODE *hash_search(HASH_NODE *hash_table[], char *cmd);

#endif