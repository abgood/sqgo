#include "sqgo.h"

void ssh(void) {
    /* declear ssh_info struct point for conn */
    conn = (ssh_point)malloc(sizeof(struct ssh_info));
    memset(conn, 0, sizeof(struct ssh_info));
}
