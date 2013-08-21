#include "sqgo.h"

/* first show */
MYSQL_RES *first_display(void) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char sql[LEN_64] = {0};
    int i = 1;

    sprintf(sql, "select * from %s", T_NAME);
    res = quiry(sql);
    if (mysql_num_rows(res) == (my_ulonglong)0) {
        printf("Don\'t find any site in %s table\n", T_NAME);
        exit(1);
    }

    printf("sq agent select\n");
    printf("%s\n", SPLIT_LINE);
    while ((row = mysql_fetch_row(res))) {
        printf("%3d - %-8s (%-2s)\n", i++, row[1], row[2]);
    }   
    printf("%s\n", SPLIT_LINE);

    return res;
}

/* second show */
void second_display(name_point agent) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char sql[LEN_256] = {0};
    int i = 1;

    sprintf(sql, "select * from %s where site like \'%s=%%\' or site like \'%%;%s=%%\'", INDEPE, agent->site_name, agent->site_name);
    res = quiry(sql);

    /* clear screen: set cursor position and clear screen */
    printf("\033[1;1H\033[2J");
    printf("sq [ %s (%s) ] area select\n", agent->site_name, agent->cn_name);

    /* show all site_id */
    while ((row = mysql_fetch_row(res))) {
        printf("%3d - %s\n", i++, row[1]);
    }
}
