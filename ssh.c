#include "sqgo.h"

/* free ssh session */
static void free_ssh_session(ssh_session session) {
    ssh_free(session);
    exit(1);
}

/* close ssh session */
static void close_ssh_session(ssh_session session) {
    ssh_disconnect(session);
    ssh_free(session);
    exit(1);
}

void ssh(void) {
    ssh_session session;
    char ip[LEN_32] = {0};
    char *banner;

    /* get server type ip */
    memset(ip, '\0', LEN_32);
    if (type_id == 1) {
        strncpy(ip, login->db1_ip, strlen(login->db1_ip));
    } else if (type_id == 2) {
        strncpy(ip, login->db2_ip, strlen(login->db2_ip));
    } else if (type_id == 3) {
        strncpy(ip, login->tel_ip, strlen(login->tel_ip));
    } else {
        printf("server type id input error!!!\n");
        exit(1);
    }

    /* ssh session create */
    if (!(session = ssh_new())) {
        fprintf(stderr, "ssh_new error\n");
        exit(1);
    }

    /* ssh session options set */
    if (strlen(ip) > 0) {
        /* set ip */
        if (ssh_options_set(session, SSH_OPTIONS_HOST, ip) < 0)
        {
            fprintf(stderr, "ssh session set ip failed\n");  
            free_ssh_session(session);
        }
        /* set user */
        if (ssh_options_set(session, SSH_OPTIONS_USER, USER) < 0)
        {
            fprintf(stderr, "ssh session set userfailed\n");  
            free_ssh_session(session);
        }     
        /* set port */
        if (ssh_options_set(session, SSH_OPTIONS_PORT_STR, PORT) < 0)
        {
            fprintf(stderr, "ssh session set port failed\n");  
            free_ssh_session(session);
        }    
    }

    /* ssh connect remote host */
    if (ssh_connect(session)) {
        fprintf(stderr, "connect failed: %s\n", ssh_get_error(session));
        close_ssh_session(session);
    }

    /* add knownhost */
    if (verify_knownhost(session) < 0) {
        fprintf(stderr, "knownhost failed\n");
        close_ssh_session(session);
    }

    /* get banner */
    if ((banner = ssh_get_issue_banner(session))) {
        printf("%s\n", banner);
        free(banner);
    }

    /* auth */
    if (authenticate_console(session) != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "authenticate failed\n");
        close_ssh_session(session);
    }

    /* open shell */
    shell(session);
}
