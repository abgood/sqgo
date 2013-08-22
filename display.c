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

/* sort link */
void sort_link(site_point p) {
    site_point site_tail;

    /* alloc site_info struct for site_head */
    if (!(site_head = malloc(sizeof(struct site_info)))) {
        printf("malloc site_info struct for site_head\n");
        exit(1);
    }

    /* alloc site_info struct for site_tail */
    site_head->next = NULL;
    site_tail = site_head;

    printf("%s %s %s %s\n", p->site, p->tel_ip, p->db1_ip, p->db2_ip);
}

/* create site link */
void create_link(MYSQL_ROW row, name_point agent) {
    char *field_start;
    char *field_end;
    char *id_start;
    char field[LEN_256];
    site_point p;

    p = (site_point)malloc(sizeof(struct site_info));
    memset(p, '\0', sizeof(struct site_info));
    memset(field, '\0', sizeof(field));

    if ((field_start = strstr(row[1], agent->site_name))) {
        if ((field_end = strstr(field_start, SEMICOLON_SPLIT))) {
            strncpy(field, field_start, field_end - field_start);
        } else {
            strcpy(field, field_start);
        }
    }

    /* store id fields to p->site */
    if ((id_start = strstr(field, EQUAL_SPLIT))) {
        strcpy(p->site, id_start + 1);
    }

    /* store to tmp node p */
    strcpy(p->tel_ip, row[2]);
    strcpy(p->db1_ip, row[6]);
    strcpy(p->db2_ip, row[7]);

    /* sort link */
    sort_link(p);
}

/* second show */
void second_display(name_point agent) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char sql[LEN_256] = {0};

    sprintf(sql, "select * from %s where site like \'%s=%%\' or site like \'%%;%s=%%\'", INDEPE, agent->site_name, agent->site_name);
    printf("%s\n", sql);
    res = quiry(sql);

    /* clear screen: set cursor position and clear screen */
    printf("\033[1;1H\033[2J");
    printf("sq [ %s (%s) ] area select\n", agent->site_name, agent->cn_name);

    /* show all site_id */
    while ((row = mysql_fetch_row(res))) {
        create_link(row, agent);
    }
}
