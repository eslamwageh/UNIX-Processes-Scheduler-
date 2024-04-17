#include "headers.h"
#include "Queue.h"
#include "PriorityQueue.h"

int time;
Process** PCBTable;
Process* runningProcess;

void switchProcess(Process* runningProcess, Process* p);
void HPF(PriorityQueuePointer readyQueue, Process* runningProcess);
void processFinishedHandler(int signum);

int main(int argc, char * argv[])
{
    initClk();
    time = -1;
    //TODO implement the scheduler :)
    int numberOfProcesses = 100;
    int algorithm = argv[0]; //1 -> HPF, 2 -> SRTN, 3 -> RR
    Process staticProcess = (Process){0, 0, 0, 0, 0, 0, 0}; // this is temporary for now as we dont have input file yet
    runningProcess = &staticProcess;
    PCBTable = malloc(numberOfProcesses * sizeof(Process*));
    PriorityQueuePointer readyQueue = priority_queue_init();
    switch (algorithm)
    {
    case 1:
        HPF(readyQueue, runningProcess);
        break;
    case 2:
        /* SRTN */
        break;
    case 3:
        /* RR */
        break;
    
    default:
        break;
    }


    //upon termination release the clock resources.
    
    destroyClk(true);
}

void switchProcess(Process* runningProcess, Process* p)
{
    if (p == NULL) return;
    if (runningProcess)
        runningProcess->state = 1;
    runningProcess = p;
    runningProcess->state = 2;
}

void HPF(PriorityQueuePointer readyQueue, Process* runningProcess) 
{
    while (time == getClk()) {};
    time = getClk();
    if (!priority_queue_empty(readyQueue)) 
    {
        switchProcess(runningProcess, priority_queue_remove(readyQueue));
        int pid = fork();
        if (pid == -1)
            perror("Error in forking a process!");
        else if (pid == 0)
        {
            char arg1[10], arg2[10], arg3[10], arg4[10], arg5[10], arg6[10], arg7[10];
            sprintf(arg1, "%d", runningProcess->id);
            sprintf(arg2, "%d", runningProcess->priority);
            sprintf(arg3, "%d", runningProcess->runTime);
            sprintf(arg4, "%d", runningProcess->executionTime);
            sprintf(arg5, "%d", runningProcess->remainingTime);
            sprintf(arg6, "%d", runningProcess->waitingTime);
            sprintf(arg7, "%d", runningProcess->state);
            execl("./process", "process", arg1, arg2, arg3, arg4, arg5, arg6, arg7, NULL);
        }
        else 
        {
            signal(SIGCHlD, processFinishedHandler);
            int statlock;
            int cid = wait(&statlock); // to ensure that it waits for the running process to finish
            if (WIFEXITED(statlock))
                printf("A process terminated successfully with status %d.", WEXITSTATUS(statlock));
            else printf("Some thing went wrong with the process.");
        }
    }
}

void processFinishedHandler(int signum)
{
    free(PCBTable[runningProcess->id]);
}
