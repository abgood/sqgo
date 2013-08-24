#include "sqgo.h"

jmp_buf jmp1;
site_point site_head;
site_point site_tail;
site_point login;
int max_id;
ssh_point conn;

int main (int argc, char **argv) {
    MYSQL_RES *agent_res;
    name_point agent;

    /* alloc name_info struct for agent */
    if (!(agent = malloc(sizeof(struct name_info)))) {
        printf("malloc name_info struct for agent\n");
        exit(1);
    }

    /* verify login password */
    // verify_pawd();

    /* show first interface */
    agent_res = first_display();

    /* set jmp point 1 */
    setjmp(jmp1);

    /* get agent chinese-name */
    get_name(agent_res, agent);

    /* show second interface */
    second_display(agent);

    /* get site id */
    get_id();
    
    /* server type select */
    select_type();

    /* ssh login */
    ssh();

    return 0;
}
