#include <mysql.h>  // 包含MySQL的头文件
#include <stdio.h>
#include <stdlib.h>

int main() {
    MYSQL *conn;  // 数据库连接句柄
    MYSQL_RES *res;  // 查询结果集
    MYSQL_ROW row;  // 查询结果行

    // 初始化MySQL连接
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    // 连接到MySQL服务器
    if (!mysql_real_connect(conn, "18.141.186.102", "root", "!QAZ2wsx@WSX1qaz", "perpetual", 0, NULL, 0)) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // 执行SQL查询
    if (mysql_query(conn, "SELECT * FROM t_D_KLine")) {
        fprintf(stderr, "mysql_query() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // 获取查询结果
    res = mysql_use_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_use_result() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // 遍历并打印查询结果
    printf("Query results:\n");
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("Column 1: %s, Column 2: %s\n", row[0], row[1]);
    }

    // 释放结果集
    mysql_free_result(res);

    // 关闭数据库连接
    mysql_close(conn);

    printf("Finished successfully!\n");
    return 0;
}