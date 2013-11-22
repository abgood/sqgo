#include "sqgo.h"

void direct_login(int argc, char **argv, name_point agent) {
    char id[LEN_8] = {'\0'};
    char type[LEN_8] = {'\0'};
    MYSQL_RES *res;

    /* server type */
    strncpy(type, *(argv + 2), strlen(*(argv + 2)));
    if (!strcasecmp(type, "db1")) {
        type_id = 1;
    } else if (!strcasecmp(type, "db2")) {
        type_id = 2;
    } else if (!strcasecmp(type, "web")) {
        type_id = 3;
    } else {
        printf("server type id input error, please input \"web, db1, db2\"\n");
        exit(1);
    }

    /* site name */
    sscanf(*(argv + 1), "%[^_]_%s", agent->site_name, id);

    /* select site name */
    res = select_site_res(agent);

    /* alloc node memory */
    alloc_node_mem();

    /* create all site_id */
    create_site_list(res, agent);
    
    /* get site info */
    get_site_info(atoi(id));

    /* test login */
    if (strlen(login->tel_ip) < 7) {
        printf("The input site does not exist, please check!!!\n");
        goto last;
    }

    /* ssh login */
    ssh();

    /* free memory */
last:  free(agent);
       free_mem();

    exit(0);
}
