#include "sqgo.h"

ssh_channel chan;
int signal_delayed=0;
struct termios terminal;

/* close session, close channel */
static void session_or_channel(ssh_session session, ssh_channel channel) {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(session);
    ssh_free(session);
    ssh_finalize();
    exit(1);
}

static void sigwindowchanged(int i) {
    (void) i;
    signal_delayed = 1;
}

/* interruput */
static void setsignal(void) {
    signal(SIGWINCH, sigwindowchanged);
    signal_delayed=0;
}

/* ptv size change */
void sizechanged(void) {
    struct winsize win = {0, 0, 0, 0};

    ioctl(1, TIOCGWINSZ, &win);
    ssh_channel_change_pty_size(chan, win.ws_col, win.ws_row);
    setsignal();
}

/* do clean */
void do_cleanup(int i) {
    (void) i;
    tcsetattr(0,TCSANOW,&terminal);
}

/* open shell */
void shell(ssh_session session) {
    ssh_channel channel;
    struct termios terminal_local;

    /* check device type, if terminal */
    int interactive = isatty(0);
    if (interactive) {
        tcgetattr(0, &terminal_local);
        memcpy(&terminal, &terminal_local, sizeof(struct termios));
    }

    /* open channel */
    channel = ssh_channel_new(session);
    if (ssh_channel_open_session(channel)) {
        printf("open channel error: %s\n", ssh_get_error(channel));
        session_or_channel(session, channel);
    }

    chan = channel;

    /* request ptv */
    if (interactive) {
        ssh_channel_request_pty(channel);
        sizechanged();
    }

    /* request shell */
    if (ssh_channel_request_shell(channel)) {
        printf("request shell: %s\n", ssh_get_error(session));
        session_or_channel(session, channel);
    }

    if (interactive) {
        cfmakeraw(&terminal_local);
        tcsetattr(0,TCSANOW,&terminal_local);
        setsignal();
    }
    signal(SIGTERM,do_cleanup);

    /* loop */
    select_loop(session, channel);

    if (interactive) {
        do_cleanup(0);
    }
}
