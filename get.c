#include "sqgo.h"

/* get site name */
void get_name(MYSQL_RES *res, name_point agent) {
    char *line;
    MYSQL_ROW row;

    /* clear struct */
    memset(agent->site_name, '\0', sizeof(agent->site_name));
    memset(agent->cn_name, '\0', sizeof(agent->cn_name));

    /* input site_name */
    while (1) {
        line = readline("please input the agent site_name: ");
        if (strlen(line) >= 2) {
            add_history(line);
            break;
        }
    }

    /* reset res point */
    mysql_data_seek(res, 0);
    while ((row = mysql_fetch_row(res))) {
        if (!strcmp(line, row[1])) {
            strncpy(agent->site_name, row[1], strlen(row[1]));
            strncpy(agent->cn_name, row[2], strlen(row[2]));
            break;
        }
    }

    if (!strlen(agent->cn_name)) {
        printf("Don\'t find any site in %s table\n\n", T_NAME);
        longjmp(jmp1, 1);
    }
}

/* get site id */
void get_id(void) {
    char *line;

    while (1) {
        line = readline("please input the agent site_id: ");
        if (atoi(line) > 0 && atoi(line) <= max_id) {
            add_history(line);
            break;
        }

        printf("please enter the correct id (0 < id <= %d)\n\n", max_id);
    }
}
