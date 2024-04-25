#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int time = -1;

int main(int agrc, char *argv[])
{
    initClk();
    remainingtime = atoi(argv[1]);
    printf("remaining time = %d\n", remainingtime);
    fflush(stdout);
    while (remainingtime > 0)
    {
        while (time == getClk())
            ;
        time = getClk();
        remainingtime--;
        printf("remaining time = %d\n", remainingtime);
        fflush(stdout);
    }

    destroyClk(false);
    return 0;
}
