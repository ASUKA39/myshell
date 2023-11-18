# 定义编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra

# 定义源文件和目标文件
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = myshell

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# 编译每个源文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)
