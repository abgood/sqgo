#include "sqgo.h"

jmp_buf jmp1;
jmp_buf jmp2;
jmp_buf jmp3;
jmp_buf jmp4;

site_point site_head;
site_point site_tail;
site_point login;

int max_id;
int enter_id;
int type_id;

int main (int argc, char **argv) {
    MYSQL_RES *agent_res;
    name_point agent;

    /* alloc name_info struct for agent */
    if (!(agent = malloc(sizeof(struct name_info)))) {
        printf("malloc name_info struct for agent\n");
        exit(1);
    }

    /* verify login password */
    verify_pawd();

    /* show first interface */
    setjmp(jmp1);
    agent_res = first_display();

    /* set jmp point 1 */
    setjmp(jmp2);

    /* get agent chinese-name */
    get_name(agent_res, agent);

    /* show second interface */
    setjmp(jmp3);
    second_display(agent);

    /* get site id */
    get_id();
    
    /* server type select */
    setjmp(jmp4);
    select_type(agent);

    /* ssh login */
    ssh();

    return 0;
}
