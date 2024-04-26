#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int timeQuantum;
int time = -1;
int q = -1;

int main(int agrc, char *argv[])
{
    initClk();

    remainingtime = atoi(argv[1]);
    if (agrc == 3)
    {
        timeQuantum = atoi(argv[2]);
        q = timeQuantum;
    }
    printf("remaining time = %d\n", remainingtime);
    fflush(stdout);
    while (remainingtime > 0)
    {
        while (time == getClk())
            ;
        printf("q is %d\n", q);
        fflush(stdout);
        time = getClk();
        remainingtime--;
        q--;
        printf("remaining time = %d\n", remainingtime);
        fflush(stdout);
        if (agrc == 3 && q == 0)
        {
            printf("I will stop now as q = 0 with pid = %d\n", getpid());
            fflush(stdout);
            kill(getppid(), SIGCONT);
            pause();
            printf("I will resume now as signal is recieved from the scheduler with pid = %d\n", getpid());
            fflush(stdout);
            q = timeQuantum;
        }
    }

    destroyClk(false);
    return 0;
}
