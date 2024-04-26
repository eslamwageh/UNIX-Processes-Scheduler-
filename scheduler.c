#include "headers.h"
#include "PriorityQueue.h"
#include "math.h"
#include "MinHeap.h"
#include <stddef.h>
#include "stdlib.h"

int time;
int algorithm;
int notBusy;
int timeQuantum;
bool inQuantum;
int msgq_id1, msgq_id2;
bool finished = false;

// Statistics
float totalWTA = 0;
float totalWTA2 = 0; // sum of WTA^2 to calculate the standard deviation
int totalWaitingTime = 0;
int totalProcesses = 0;
int totalProcessesFinished = 0;
int totalExecutionTime = 0;
FILE *logFile;

Process **PCBTable;
Process *runningProcess;
priority_queue *readyQueue;
PriorityQueuePointer rrReadyQueue;

typedef struct msgbuff
{
    long mtype;
    Process msg_process;
} msgbuff;

void switchProcess(Process **runningProcess, Process *p);
void SRTN();
void HPF();
void processFinishedHandler(int signum);
void allProcessesSentHandler(int signum);
void finishedQuantum(int signum);
void deleteProcess();
void RR(int timeQuantum);
void receiveProcess(int signum);

void writeToLogFile(int state); // 0 -> runs, 1 -> stops, 2 -> finishes
void createPerfFile();

int main(int argc, char *argv[])
{
    signal(SIGCHLD, processFinishedHandler);
    signal(SIGUSR1, allProcessesSentHandler);
    signal(SIGUSR2, receiveProcess);
    signal(SIGCONT, finishedQuantum);

    initClk();
    logFile = fopen("scheduler.log", "w");
    time = -1;
    // TODO implement the scheduler :)
    totalProcesses = atoi(argv[3]);
    algorithm = atoi(argv[1]);
    timeQuantum = atoi(argv[2]); // 1 -> HPF, 2 -> SRTN, 3 -> RR
    runningProcess = NULL;
    PCBTable = malloc(totalProcesses * sizeof(Process *));

    printf("Scheduler started with arguments: (%d, %d, %d)\n", algorithm, timeQuantum, totalProcesses);
    msgq_id1 = getMessageQueueID("pgen_sch_keyfile", 65);
    msgq_id2 = getMessageQueueID("pgen_sch_keyfile", 66);
    if (msgq_id1 == -1)
    {
        perror("Error in creating the queue");
        exit(-1);
    }
    printf("Message queue id =    %d\n", msgq_id1);

    switch (algorithm)
    {
    case 1:
        readyQueue = createMinHeap(HPF_Algorithm);
        HPF(readyQueue);
        break;
    case 2:
        /* SRTN */
        readyQueue = createMinHeap(SRTN_Algorithm);
        SRTN();
        break;
    case 3:
        // readyQueue = createMinHeap(RR_Algorithm);
        rrReadyQueue = priority_queue_init();
        RR(timeQuantum);
        break;

    default:
        break;
    }
    fclose(logFile);
    createPerfFile();
    destroyClk(false);
    printf("Scheduler finished\n");
    exit(0);
    return 0;
}

// no longer 3arf eh lazmet el function de
void switchProcess(Process **runningProcess, Process *p)
{
    bool firstTime = false;
    if (*runningProcess == NULL)
        firstTime = true;
    if (p == NULL)
        return;
    if (*runningProcess)
        (*runningProcess)->state = 1;
    *runningProcess = p;
    if (!firstTime)
        (*runningProcess)->state = 2;
}

void SRTN()
{
}

void HPF()
{
    while (!finished)
    {
        while (!isHeapEmpty(readyQueue))
        {
            printHeap(readyQueue);
            while (time == getClk())
            {
            };
            time = getClk();
            if (!(runningProcess))
            {
                switchProcess((&runningProcess), popFromHeap(readyQueue));
                runningProcess->waitingTime = time - runningProcess->arrivalTime;
                writeToLogFile(0);
                int pid = fork();
                if (pid == -1)
                    perror("Error in forking a process!");
                else if (pid == 0)
                {
                    char arg1[10];
                    sprintf(arg1, "%d", (runningProcess)->remainingTime);
                    execl("./process.out", "process", arg1, NULL);
                }
                else
                {
                    int statlock;
                    // we must wait for all processes to finish not only the first//////////////////////////
                    int cid = wait(&statlock); // to ensure that it waits for the running process to finish
                    if (WIFEXITED(statlock))
                        printf("A process terminated successfully with status %d.", WEXITSTATUS(statlock));
                    else
                        printf("Some thing went wrong with the process.");
                    fflush(stdout);
                }
            }
        }
    }
}

void RR(int timeQuantum)
{
    while (totalProcessesFinished < totalProcesses)
    {
        while (time == getClk())
            ;
        time = getClk();
        printf("waiting for ready queue\n");
        fflush(stdout);
        if (!priority_queue_empty(rrReadyQueue))
        {
            inQuantum = true;
            runningProcess = priority_queue_remove(rrReadyQueue);
            printf("\n\nrunning process %d\n\n\n", runningProcess->id);
            fflush(stdout);
            if (runningProcess->state == 0)
            {
                printf("forking process %d\n", runningProcess->id);
                fflush(stdout);
                int pid = fork();
                if (pid == -1)
                    perror("Error in forking a process!");
                else if (pid == 0)
                {
                    char arg1[10], arg2[10];
                    sprintf(arg1, "%d", runningProcess->remainingTime);
                    sprintf(arg2, "%d", timeQuantum);
                    execl("./process.out", "process", arg1, arg2, NULL);
                }
                else
                {
                    runningProcess->pid = pid;
                }
            }
            else
            {
                printf("no need to fork process %d\n", runningProcess->id);
                fflush(stdout);
            }
            writeToLogFile(0);
            runningProcess->state = 2;
            runningProcess->waitingTime += time - runningProcess->lastStoppedTime;

            kill(runningProcess->pid, SIGUSR1);
            printf("time before while : %d", getClk());
            while (inQuantum)
            {
            }
            printf("time after while : %d", getClk());
            if (runningProcess->remainingTime > 0)
            {
                insert_into_tail(runningProcess, rrReadyQueue);
                runningProcess->lastStoppedTime = time;
                runningProcess->state = 1;
                writeToLogFile(1);
            }
        }
    }
    printf("exited the big while");
}
// 1. generator sends process to scheduler
// 2. scheduler fork the process and store its pid
// 3. scheduling algorithm let the process run and decrement its remaining time

void receiveProcess(int signum)
{
    printf("received signal from generator\n");
    fflush(stdout);
    Process *p = malloc(sizeof(Process));
    msgbuff message;
    int recval = msgrcv(msgq_id1, &message, sizeof(message.msg_process), 0, !IPC_NOWAIT);
    *p = message.msg_process;
    if (recval == -1)
    {
        perror("Error in receiving from generator");
        fflush(stdout);
    }
    else
    {
        printf("recieved with arrival time : %d\n", p->arrivalTime);
        fflush(stdout);
        totalExecutionTime += p->runTime;
        p->lastStoppedTime = getClk();
        PCBTable[p->id] = p;
        switch (algorithm)
        {
        case 1:
            pushInHeap(readyQueue, p);
            printf("pushed in heap\n");
            fflush(stdout);
            break;
        case 2:
            // pushInHeap(readyQueue, p);
            break;
        case 3:
            printf("inserting process %d into ready queue\n", p->id);
            insert_into_tail(p, rrReadyQueue);
            break;
        }

        printf("minHeap\n");
        fflush(stdout);
        // printHeap(readyQueue);

        // print ready queue content
        // if (rrReadyQueue->Count == 5)
        //     for (int i = 0; !priority_queue_empty(rrReadyQueue); i++)
        //     {
        //         printf("rrReadyQueue : I am process %d with id : %d\n", i, priority_queue_remove(rrReadyQueue)->id);
        //         fflush(stdout);
        //     }
        int sendval = msgsnd(msgq_id2, &message, sizeof(message.msg_process), !IPC_NOWAIT);
        printf("--------------------------------------------------------------------------------\n");
        fflush(stdout);
    }
    signal(SIGUSR2, receiveProcess);
}

void writeToLogFile(int state)
{
    if (logFile == NULL)
    {
        perror("Error in opening the log file");
        exit(-1);
    }
    time = getClk();
    switch (state)
    {
    case 0:
        if ((runningProcess)->state == 0)
            fprintf(logFile, "At time %d process %d started arr %d total %d remain %d wait %d\n", time, (runningProcess)->id, runningProcess->arrivalTime, runningProcess->runTime, runningProcess->remainingTime, runningProcess->waitingTime);
        else
            fprintf(logFile, "At time %d process %d resumed arr %d total %d remain %d wait %d\n", time, runningProcess->id, runningProcess->arrivalTime, runningProcess->runTime, runningProcess->remainingTime, runningProcess->waitingTime);
        break;
    case 1:
        fprintf(logFile, "At time %d process %d stopped arr %d total %d remain %d wait %d\n", time, runningProcess->id, runningProcess->arrivalTime, runningProcess->runTime, runningProcess->remainingTime, runningProcess->waitingTime);
        break;
    case 2:
        fprintf(logFile, "At time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %f\n", time + 1, runningProcess->id, runningProcess->arrivalTime, runningProcess->runTime, runningProcess->remainingTime, runningProcess->waitingTime, time - runningProcess->arrivalTime + 1, (float)(time - runningProcess->arrivalTime + 1) / runningProcess->runTime);
        totalWTA += (float)(time - (runningProcess)->arrivalTime) / (runningProcess)->runTime;
        totalWTA2 += pow((float)(time - (runningProcess)->arrivalTime) / (runningProcess)->runTime, 2);
        totalWaitingTime += (runningProcess)->waitingTime;
        break;
    }
}

void processFinishedHandler(int signum)
{
    totalProcessesFinished++;
    runningProcess->remainingTime = 0;
    writeToLogFile(2);
    deleteProcess();
    signal(SIGCHLD, processFinishedHandler);
}

void createPerfFile()
{
    FILE *perfFile = fopen("scheduler.perf", "w");
    if (perfFile == NULL)
    {
        perror("Error in opening the performance file");
        exit(-1);
    }
    time = getClk();
    fprintf(perfFile, "CPU utilization = %f\n", (float)totalExecutionTime / (time - notBusy));
    fprintf(perfFile, "Avg WTA = %f\n", totalWTA / totalProcesses);
    fprintf(perfFile, "Std WTA = %f\n", sqrt((totalWTA2 / totalProcesses) - pow(totalWTA / totalProcesses, 2)));
    fprintf(perfFile, "Avg Waiting = %f\n", (float)totalWaitingTime / totalProcesses);
    fclose(perfFile);
}

void deleteProcess()
{
    free(PCBTable[runningProcess->id]);
    runningProcess = NULL;
}

void allProcessesSentHandler(int signum)
{
    finished = true;
    signal(SIGUSR1, allProcessesSentHandler);
}

void finishedQuantum(int signum)
{
    inQuantum = false;
    runningProcess->remainingTime -= timeQuantum;
    signal(SIGCONT, finishedQuantum);
}