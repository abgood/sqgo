# 编译选项
CC = gcc
CFLAGS = -g -O2 -Wall

# 库
LIBMYSQL = `mysql_config --libs`
LIBSSL = -lssl
LIBREADLINE = -lreadline -lncurses
LIBPCRE = -lpcre

# 执行文件
TARGET = sqgo

# 所有的c文件生成o文件
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 把所有以.c结尾的文件放到列表里
SOURCES = $(wildcard *.c)
# 匹配sources列表里所有.c文件替换成.o文件放到列表里
OBJS = $(patsubst %.c,%.o,$(SOURCES))

# 生成执行文件
$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBMYSQL) $(LIBSSL) $(LIBREADLINE) $(LIBPCRE)
	chmod u+x $@

# 删除.o文件和执行文件
clean:
	rm $(OBJS) $(TARGET) -f
