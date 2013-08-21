#ifndef SQGO_GET_H
#define SQGO_GET_H

#include "define.h"

typedef struct name_info {
    char site_name[LEN_16];
    char cn_name[LEN_32];
} *name_point;

/* get agent chinese-name */
void get_name(MYSQL_RES *, name_point);

#endif
