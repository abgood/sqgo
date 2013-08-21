#include "sqgo.h"

MYSQL_RES *quiry(char *sql) {
    MYSQL *conn;
    MYSQL_RES *res;
    char *db_str = "6BBDE8BE1EB0A0A8D36FB5ED041721183739DCCC6A382AF776C44CAB6C8FE63A";
    char *db_pawd;

    char *ip = "113.107.167.90";
    int port = 2433;
    db_pawd = aes_dec(db_str, 0);

    conn = mysql_init(NULL);

    if (!(mysql_real_connect(conn, ip, "sq_check", db_pawd, "sq", port, NULL, 0))) {
        if (mysql_error(conn))
            fprintf(stderr, "connection error %d : %s\n", mysql_errno(conn), mysql_error(conn));
        printf("Fail to connect mysql, ip:%s\tport:%d\n", ip, port);
    }

    // 程序连接编码设置
    // mysql_set_character_set(conn, "utf8");
    mysql_query(conn, "set names utf8");

    // 查询数据库
    if (mysql_query(conn,sql))
    {
        fprintf(stderr, "查询失败\n");
        return NULL;
    }
    // 生成结果集
    if (!(res=mysql_store_result(conn)))
    {
        fprintf(stderr, "结果集生成失败\n");
        return NULL;
    }

    mysql_close(conn);
    return res;
}
