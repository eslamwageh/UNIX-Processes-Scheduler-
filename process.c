#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int time = -1;
int schedulerProcessSharedMemoryID;
int* schedulerProcessSharedMemoryAddress;


int main(int agrc, char *argv[])
{
    initClk();
    schedulerProcessSharedMemoryID = getSharedMemory("sch_pcs_keyfile",'A');
    schedulerProcessSharedMemoryAddress = (int*) getSharedMemoryAddress(schedulerProcessSharedMemoryID);
    remainingtime = atoi(argv[1]);
    *schedulerProcessSharedMemoryAddress = remainingtime;
    printf("remaining time = %d\n", remainingtime);
    fflush(stdout);
    while (remainingtime > 0)
    {
        while (time == getClk())
            ;
        time = getClk();
        remainingtime--;
        *(schedulerProcessSharedMemoryAddress) = remainingtime;
        printf("remaining time = %d\n", remainingtime);
        fflush(stdout);
    }
    kill(getppid(),SIGTSTP);
    destroyClk(false);
    return 0;
}
