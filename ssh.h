#ifndef SQGO_SSH_H
#define SQGO_SSH_H

#include <libssh/libssh.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

/* ssh struct */
typedef struct ssh_info {
    ssh_session session;
    ssh_channel channel;
} *ssh_point;

/* ssh connect */
void ssh(void);

#endif
