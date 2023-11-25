#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>

#include "parse.h"

// syntax tree structure example:
//                      __ PIPELINE__
//                  ___/              \____
//                 /                       \
//         COMMAND                    __ PIPELINE _
//       /        \                  /             \
// ARGUMENTS   REDIRECTIONS      COMMAND         _ COMMAND __
//     |          |     |           |           /            \
//    cat        <<     >       ARGUMENTS    ARGUMENTS   REDIRECTIONS
//                |     |         |   |      |   |   |        |
//              "..."  file      wc  -c      tr  -d " "       >
//                                                            |
//                                                          file2

// 解析命令行
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

// 解析语法树节点

// 解析命令节点

// 解析管道节点

// 解析重定向节点

// 执行语法树节点

// 中序遍历语法树