#ifndef SQGO_SSH_H
#define SQGO_SSH_H

#include <termios.h>
#include <pty.h>
#include <fcntl.h>
#include <signal.h>

#include <libssh/libssh.h>
#include <libssh/callbacks.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "define.h"

/* ssh connect */
void ssh(void);

/* add knownhost */
int verify_knownhost(ssh_session);

/* auth */
int authenticate_console(ssh_session);

/* open shell */
void shell(ssh_session);

/* loop */
void select_loop(ssh_session, ssh_channel);

/* do cleanup */
void do_cleanup(int);

/* size change */
void sizechanged(void);

#endif
