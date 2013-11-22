#include "sqgo.h"

jmp_buf jmp5;
agent_point agent_head, agent_tail;

static void create_list(MYSQL_ROW row) {
    /* new create tmp node */
    agent_point p = (agent_point)malloc(sizeof(struct agent_info));
    memset(p, '\0', sizeof(struct agent_info));

    /* copy data */
    strcpy(p->id, row[0]);
    strcpy(p->site, row[1]);
    strcpy(p->agent, row[2]);

    /* insert node */
    agent_tail->next = p;
    agent_tail = p;
}

/* copy data to agent_info */
static void copy_to_agent(agent_point p, name_point agent) {
    strcpy(agent->site_name, p->site);
    strcpy(agent->cn_name, p->agent);
}

/* second input select */
static void second_select(name_point agent) {
    char *line;
    agent_point p = agent_head->next;
    int flag = 0;

    while ((line = readline("please input the full agent (id, site, agent): "))) {
        if (strlen(line) >= 1) {
            add_history(line);
            break;
        }
    }

    while (p) {
        /* id */
        if (!strcmp(line, p->id)) {
            copy_to_agent(p, agent);
            flag++;
            break;
        }

        /* site */
        if (!strcmp(line, p->site)) {
            copy_to_agent(p, agent);
            flag++;
            break;
        }

        /* agent */
        if (!strcmp(line, p->agent)) {
            copy_to_agent(p, agent);
            flag++;
            break;
        }

        /* next node */
        p = p->next;
    }

    if (flag == 0) {
        printf("\nPlease enter the full contents of the following\n");
        longjmp(jmp5, 1);
    }
}

/* release list */
static void release_agent_info(void) {
    agent_point p = agent_head->next;

    while (p) {
        free(p);
        p = p->next;
    }
}

/* get site name */
void get_name(MYSQL_RES *res, name_point agent) {
    char *line;
    MYSQL_ROW row;
    int agent_num = 0;

    /* init agent_info node */
    agent_head = (agent_point)malloc(sizeof(agent_point));
    agent_tail = agent_head;
    agent_tail->next = NULL;

    /* clear struct */
    memset(agent->site_name, '\0', sizeof(agent->site_name));
    memset(agent->cn_name, '\0', sizeof(agent->cn_name));

    /* input site_name */
    while (1) {
        line = readline("please input the agent (id, site, agent): ");
        if (strlen(line) >= 1) {
            add_history(line);
            break;
        }
    }

    setjmp(jmp5);
    printf("%s\n", SPLIT_LINE);
    /* reset res point */
    mysql_data_seek(res, 0);
    while ((row = mysql_fetch_row(res))) {
        /* id */
        if (!strcmp(line, row[0])) {
            strncpy(agent->site_name, row[1], strlen(row[1]));
            strncpy(agent->cn_name, row[2], strlen(row[2]));
            break;
        } else {
            /* site */
            if (strstr(row[1], line)) {
                printf("%3s - %-8s (%-2s)\n", row[0], row[1], row[2]);
                create_list(row);
                agent_num++;
                continue;
            }

            /* agent */
            if (strstr(row[2], line)) {
                printf("%3s - %-8s (%-2s)\n", row[0], row[1], row[2]);
                create_list(row);
                agent_num++;
                continue;
            }
        }
    }
    printf("%s\n", SPLIT_LINE);

    if (agent_num != 0) {
        if (agent_num == 1) {
            agent_point p = agent_head->next;
            copy_to_agent(p, agent);
        } else {
            second_select(agent);
        }
    }

    release_agent_info();

    if (!strlen(agent->cn_name)) {
        printf("Don\'t find any site in %s table\n\n", T_NAME);
        longjmp(jmp2, 1);
    }
}

/* get ip info */
int get_ip(site_point p, int input_id) {
    char *id_start;
    char id_token_start[LEN_512] = {0};
    char id_token_end[LEN_512] = {0};
    char id_start_tmp[LEN_512] = {0};
    char *token;
    int int_id_start;
    int int_id_end;

    if ((id_start = strstr(p->site, MINUS_SPLIT))) {
        id_start += 1;
    } else {
        id_start = p->site;
    }

    login = (site_point)malloc(sizeof(struct site_info));
    memset(login, '\0', sizeof(struct site_info));

    /* if have not "~", compare id fields */
    if (!strstr(id_start, TILDE_SPLIT)) {
        if (input_id == atoi(id_start)) {
            strcpy(login->site, p->site);
            strcpy(login->tel_ip, p->tel_ip);
            strcpy(login->db1_ip, p->db1_ip);
            strcpy(login->db2_ip, p->db2_ip);
            return 1;
        }
    } else {
        memset(id_start_tmp, '\0', LEN_512);
        strncpy(id_start_tmp, id_start, strlen(id_start));
        token = strtok(id_start_tmp, COMMA_SPLIT);
        while (token) {
            memset(id_token_start, '\0', LEN_512);
            memset(id_token_end, '\0', LEN_512);
            sscanf(token, "%[^~]~%[^;];", id_token_start, id_token_end);

            int_id_start = atoi(id_token_start);
            int_id_end = atoi(id_token_end);

            /* get ip */
            if ((input_id >= int_id_start && input_id <= int_id_end) || (input_id == int_id_start)) {
                strcpy(login->site, p->site);
                strcpy(login->tel_ip, p->tel_ip);
                strcpy(login->db1_ip, p->db1_ip);
                strcpy(login->db2_ip, p->db2_ip);
                return 1;
            }

            token = strtok(NULL, COMMA_SPLIT);
        }
    }

    return 0;
}

/* get site id */
void get_id(void) {
    char *line;
    site_point p = malloc(sizeof(struct site_info));

    /* input site id */
    while (1) {
        line = readline("please input the agent site_id: ");
        enter_id = atoi(line);
        if ((enter_id > 0 && enter_id <= max_id) || !strcasecmp(line, "r")) {
            if (!strcasecmp(line, "r")) {
                longjmp(jmp1, 1);
            }
            add_history(line);
            break;
        }

        printf("please enter the correct id (0 < id <= %d)\n\n", max_id);
    }

    /* get site id info */
    p = site_head->next;
    while (p) {
        if (get_ip(p, enter_id) == 1) {
            break;
        }
        p = p->next;
    }
}

/* free memory */
void free_mem(void) {
    free(site_head);
    free(site_tail);
    free(login);
}
