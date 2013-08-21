#include "sqgo.h"

void verify_pawd(void) {
    char *pawd;
    int i = 1;

    while (1) {
        pawd = getpass("please input the password: ");
        if (strcmp(SQ_PAWD, pawd) == 0) {
            break;
        }
        if (i++ == 3) {
            printf("bye\n");
            exit(1);
        }
        printf("input password error!\n\n");
    }

    return;
}
