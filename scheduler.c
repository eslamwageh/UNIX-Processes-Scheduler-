#include "headers.h"
#include "Queue.h"

typedef struct Process
{
    int id;
    int priority;
    int arrivalTime;
    int runTime;
    int executionTime;
    int remainingTime;
    int waitingTime;
    int state; // 0: waiting, 1: ready, 2: running 
} Process;

void switchProcess(Process p);

int main(int argc, char * argv[])
{
    initClk();
    
    //TODO implement the scheduler :)
    int numberOfProcesses = 100;
    Process staticProcess = (Process){0, 0, 0, 0, 0, 0, 0}; // this is temporary for now as we dont have input file yet
    Process* runningProcess = &staticProcess;
    Process** PCBTable = malloc(numberOfProcesses * sizeof(Process*));


    //upon termination release the clock resources.
    
    destroyClk(true);
}

void switchProcess(Process* p)
{
    runningProcess->state = 1;
    runningProcess = p;
    runningProcess->state = 2;
}