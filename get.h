#ifndef SQGO_GET_H
#define SQGO_GET_H

#include "define.h"

/* name info struct */
typedef struct name_info {
    char site_name[LEN_16];     /* site EN */
    char cn_name[LEN_32];       /* site CN */
} *name_point;

/* agent info struct */
typedef struct agent_info {
    char id[LEN_8];             /* agent number */
    char site[LEN_16];          /* site EN */
    char agent[LEN_32];         /* site CN */
    struct agent_info *next;    /* next node point */
} *agent_point;

/* site info struct */
typedef struct site_info {
    char site[LEN_256];         /* site field */
    char tel_ip[LEN_32];        /* telecom ip */
    char db1_ip[LEN_32];        /* db1 ip */
    char db2_ip[LEN_32];        /* db2 ip */
    struct site_info *next;     /* next node point */
} *site_point;

/* get agent chinese-name */
void get_name(MYSQL_RES *, name_point);

/* get site id */
void get_id(void);

/* free memory */
void free_mem(void);

/* get site info */
void get_site_info(int);

/* update site info */
void update_info(void);

#endif
