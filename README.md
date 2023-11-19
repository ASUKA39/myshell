# myshell
TODO:
- man
- ps: `ps -e` `ps -ef` `ps -aux`
- tree: print tree structure still has some bugs
- redirection
- pipe
- background

## development template
function template:
```c
#include "cmd_template.h"

int cmd_template(int argc, char *argv[]) {
    if (argc < n) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }
    // TODO
    return 0;
}
```
header template:
```c
#ifndef CMD_TEMPLATE_H_
#define CMD_TEMPLATE_H_

int cmd_template(int argc, char *argv[]);

#endif
```
add header to `main.h`:
```c
#include "cmd_template.h"
```
register command in `main.c/regist_func()`:
```c
static HASH_NODE *regist_func(void) {
    // other commands
    hash_register(hash_table, "cmd_template", cmd_template);
    return NULL;
}
```