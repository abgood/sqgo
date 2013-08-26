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

/* match last site id */
void do_pcre(char *site) {
    pcre *re;
    char *pattern = "\\d+$";
    const char *error;
    int erroffset, rc;
    int ovector[LEN_32];

    char *substring_start;
    int substring_length;

    re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
    if (re == NULL) {
        printf("pcre compilation failed at offset %d : %s\n", erroffset, error);
        exit(1);
    }

    rc = pcre_exec(re, NULL, site, strlen(site), 0, 0, ovector, LEN_32);
    if(rc < 0){
        if(rc == PCRE_ERROR_NOMATCH) {
            printf("sorry, no match ...\n");
        } else {
            printf("matching error %d\n", rc);  
            pcre_free(re);  
            exit(1);
        }
    }

    /*
    substring_start = site + ovector[2 * i];
    substring_length = ovector[2 * i + 1] - ovector[2 * i];
    */
    substring_start = site + ovector[0];
    substring_length = ovector[1] - ovector[0];

    max_id = atoi(substring_start);
}

/* show all site id */
void show_all_site_id(name_point agent) {
    int i = 1;
    site_point p = malloc(sizeof(struct site_info));
    p = site_head->next;

    printf("%s%s\n", SPLIT_LINE, SPLIT_LINE);
    while (p) {
        printf("%3d - %s_%-15s\t(%s*%s)\n", i++, agent->site_name, p->site, agent->cn_name, p->site);
        do_pcre(p->site);
        p = p->next;
    }
    printf("%s%s\n", SPLIT_LINE, SPLIT_LINE);
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

/* server type select */
void select_type(name_point agent) {
    char *line;

    /* clear screen: set cursor position and clear screen */
    printf("\033[1;1H\033[2J");
    printf("sq [ %s %d ] server select\n", agent->site_name, enter_id);
    printf("%s\n1 - DB1Server\n2 - DB2Server\n3 - WebServer\n", SPLIT_LINE);
    printf("%s\n", SPLIT_LINE);

    /* input server type */
    while (1) {
        line = readline("please enter the select server type: ");
        type_id = atoi(line);
        if (type_id >= 1 && type_id <= 3) {
            add_history(line);
            break;
        }

        printf("please enter the correct id (1 <= id <= 3)\n\n");
    }
}
