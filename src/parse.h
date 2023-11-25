#ifndef PARSE_H__
#define PARSE_H__

struct cmd_st {
    glob_t globres;
};

// 构建语法树
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

// 语法树节点
// 1. 命令节点
// 2. 管道节点
// 3. 重定向节点

// 语法树节点类型
enum {
    NODE_COMMAND,
    NODE_PIPELINE,
    NODE_REDIRECT,
};

// 重定向类型
enum {
    REDIRECT_IN = 1,
    REDIRECT_OUT,
    REDIRECT_APPEND,
};

// 管道节点
typedef struct pipline_st {
    int type; // 节点类型
    struct pipline_st *left, *right; // 左右子树
} PIPLINE;

// 命令节点
typedef struct cmd_st {
    int argc;
    char *argv[];
} ARGUMENTS;

// 重定向节点
typedef struct redirect_st {
    int type; // 节点类型
    int redirect_type; // 重定向类型
    char *file; // 文件名
    struct redirect_st *next; // 下一个重定向节点
} REDIRECT;

// CMD节点
typedef struct cmd_st {
    int type; // 节点类型
    ARGUMENTS *args; // 命令参数
    REDIRECT *redirects; // 重定向
} CMD;

// 语法树节点
typedef struct node_st {
    int type; // 节点类型
    union {
        CMD cmd;
        PIPLINE pipline;
        REDIRECT redirect;
    };
} ASTNODE;

#endif