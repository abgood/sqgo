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
// void sort_link(site_point p) {
// }

/* create site link */
void create_link(MYSQL_ROW row, name_point agent) {
    char *field_start;
    char *field_end;
    char *id_start;
    char field[LEN_256];
    site_point p;

    /* alloc site_info struct for p */
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

    /* insert node */
    site_tail->next = p;
    site_tail = p;
}

/* show all site id */
void show_all_site_id(name_point agent) {
    int i = 1;
    site_point p = malloc(sizeof(struct site_info));
    p = site_head->next;

    while (p) {
        printf("%3d - %s_%-15s\t(%s*%s)\n", i++, agent->site_name, p->site, agent->cn_name, p->site);
        p = p->next;
    }
}

/* sort site id */
void sort_link(void) {
    site_point p, q, s, tail;
    tail = NULL;
    while(site_head->next != tail) {
        p = site_head;
        q = p->next;
        while(q->next != tail) {
            if(atoi(p->next->site) > atoi(q->next->site)) {
                s = q->next;
                p->next = q->next;
                q->next = q->next->next;
                p->next->next = q;
                q = s;
            }
            p = p->next;
            q = q->next;
        }
        tail = q;
    }
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

    /* malloc some struct memory */
    site_head = (site_point)malloc(sizeof(struct site_info));
    site_tail = (site_point)malloc(sizeof(struct site_info));
    memset(site_head, '\0', sizeof(struct site_info));
    memset(site_tail, '\0', sizeof(struct site_info));
    site_tail = site_head;
    // site_head->next = site_tail;

    /* show all site_id */
    while ((row = mysql_fetch_row(res))) {
        create_link(row, agent);
    }
    site_tail->next = NULL;

    /* sort site id */
    sort_link();

    /* show all site id */
    show_all_site_id(agent);
}
