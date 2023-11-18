#ifndef HASH_H__
#define HASH_H__

int hash_register(const char *name, int (*func)(int, char **));
int hash_search(const char *name);

#endif