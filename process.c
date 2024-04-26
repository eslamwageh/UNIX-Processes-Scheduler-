#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int timeQuantum;
int time = -1;
int schedulerProcessSharedMemoryID;
int *schedulerProcessSharedMemoryAddress;

void slamo3leko(int signum);

int q = -1;

int main(int agrc, char *argv[])
{
    signal(SIGUSR1, slamo3leko);
    initClk();
    schedulerProcessSharedMemoryID = getSharedMemory("sch_pcs_keyfile", 'A');
    schedulerProcessSharedMemoryAddress = (int *)getSharedMemoryAddress(schedulerProcessSharedMemoryID);
    remainingtime = atoi(argv[1]);
    *schedulerProcessSharedMemoryAddress = remainingtime;
    if (agrc == 3)
    {
        timeQuantum = atoi(argv[2]);
        q = timeQuantum;
    }
    printf("starting remaining time = %d\n", remainingtime);
    fflush(stdout);
    while (remainingtime > 0)
    {
        time = getClk();
        while (time == getClk())
            ;
        printf("q is %d\n", q);
        fflush(stdout);

        remainingtime--;
        *(schedulerProcessSharedMemoryAddress) = remainingtime;
        q--;
        printf("remaining time = %d\n", remainingtime);
        fflush(stdout);
        if (remainingtime > 0 && agrc == 3 && q == 0)
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
    kill(getppid(), SIGTSTP);
    destroyClk(false);
    return 0;
}

void slamo3leko(int signum)
{
    printf("slamo 3leko\n");
}
