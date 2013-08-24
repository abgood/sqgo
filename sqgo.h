#ifndef SQGO_H
#define SQGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <setjmp.h>
#include <pcre.h>

#include <openssl/aes.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <mysql/mysql.h>

#include "verify.h"
#include "quiry.h"
#include "aes_code.h"
#include "display.h"
#include "get.h"
#include "ssh.h"

extern jmp_buf jmp1;
extern site_point site_head;
extern site_point site_tail;
extern site_point login;
extern int max_id;
extern ssh_point conn;

#endif
