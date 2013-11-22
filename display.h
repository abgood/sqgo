#ifndef SQGO_DISPLAY_H
#define SQGO_DISPLAY_H

#include "get.h"

/* first show */
MYSQL_RES *first_display(void);

/* second show */
void second_display(name_point);

/* server type select */
void select_type(name_point);

/* select site res */
MYSQL_RES *select_site_res(name_point);

/* alloc node memory */
void alloc_node_mem(void);

/* create all site list */
void create_site_list(MYSQL_RES *, name_point);

#endif
