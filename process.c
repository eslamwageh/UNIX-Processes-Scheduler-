#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char *argv[])
{
    initClk();
    remainingtime = atoi(argv[1]);
    printf("remaining time = %d\n", remainingtime);
    fflush(stdout);
    while (remainingtime > 0)
    {
        raise(SIGSTOP);
        remainingtime--;
        printf("remaining time = %d\n", remainingtime);
        fflush(stdout);
    }

    destroyClk(false);
    return 0;
}
