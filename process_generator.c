#include "headers.h"
#include "PriorityQueue.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgq_id;
void clearResources(int);

typedef struct msgbuff
{
    long mtype;
    Process msg_process;
} msgbuff;

int sendval;
<<<<<<< Updated upstream
int processesCount = -1;
=======
int processesCount = 0;
int schedPid;
>>>>>>> Stashed changes
Process *processes;

void readInputFile();
Process _createProcess(int id, int arrivalTime, int runTime, int priority);
void sendProcessToScheduler(Process p);
void getUserInput(int *algorithm, int *parameter);
void createScheduler(int algorithm, int parameter);
void createClock();


int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    readInputFile();

    //! testing reading the file
    // for (int i = 0; i < processesCount; i++)
    // {
    //     printf("Process %d: id = %d, arrivalTime = %d, runTime = %d, priority = %d\n", i, processes[i].id, processes[i].arrivalTime, processes[i].runTime, processes[i].priority);
    // }

    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int algorithm, parameter;
    getUserInput(&algorithm, &parameter);
    // 3. Initiate and create the scheduler and clock processes.
    createScheduler(algorithm, parameter);
    createClock();
    int ptr = 0;
    while (ptr < processesCount)
    {
        if (getClk() == processes[ptr].arrivalTime)
        {
            sendProcessToScheduler(processes[ptr]);
            ptr++;
        }
    }
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
<<<<<<< Updated upstream
    key_t key_id;

    key_id = ftok("pgen_sch_keyfile", 65);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);

    if (msgq_id == -1)
    {
        perror("Error in creating the queue");
        exit(-1);
    }
    printf("Message queue id = %d\n", msgq_id);
=======
    
    msgq_id = getMessageQueueID("pgen_sch_keyfile",65);

    int ptr = 0;
    printf("mohammed");
    printf("arrival of first process is : %d", processes[ptr].arrivalTime);
    while (ptr < processesCount)
    {
        if (getClk() - x == processes[ptr].arrivalTime)
        {
            sendProcessToScheduler(processes[ptr]);
            ptr++;
        }
    }
>>>>>>> Stashed changes

    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    // TODO Clears all resources in case of interruption
    struct msqid_ds ctl_statud_ds;
    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *)0);
    kill(-getpgrp(), SIGKILL);
}

void sendProcessToScheduler(Process p)
{
    msgbuff message;
    message.mtype = 1;
    message.msg_process = p;
    kill(getppid(), SIGUSR2);
    sendval = msgsnd(msgq_id, &message, sizeof(message.msg_process), !IPC_NOWAIT);

    if (sendval == -1)
    {
        perror("Error in sending to scheduler");
    }
}

void getUserInput(int *algorithm, int *parameter)
{
    scanf("Please Enter the type of the algoritm: \n 1: HPF \n 2: SRTN \n 3: RR %d", algorithm);
    if (algorithm == 3)
        scanf("Please Enter the time step: %d", parameter);
}

void createScheduler(int algorithm, int parameter)
{
    int pid = fork();
    if (pid == -1)
        perror("Error in forking a process!");
    else if (pid == 0)
    {
        char arg1[10], arg2[10];
        sprintf(arg1, "%d", algorithm);
        sprintf(arg2, "%d", parameter);
        execl("./scheduler", "scheduler", arg1, arg2, NULL);
    }
    else
    {
    }
}

void createClock()
{
    int pid = fork();
    if (pid == -1)
        perror("Error in forking a process!");
    else if (pid == 0)
    {
        execl("./clk", "clk", NULL);
    }
    else
    {
    }
}

void readInputFile()
{
    FILE *file = fopen("processes.txt", "r");
    int id , arrivaltime , runtime , priority;
    char line[100];
    fgets(line,sizeof(line),file);
    while (fscanf(file, "%d\t%d\t%d\t%d\n", &id, &arrivaltime, &runtime, &priority) == 4)
        processesCount++;
    fclose(file);
    file = fopen("processes.txt","r");
    fgets(line,sizeof(line),file);
    processes = (Process *)malloc(processesCount * sizeof(Process));
    for(int i = 0 ; i < processesCount;i++){
        fscanf(file, "%d %d %d %d", &id, &arrivaltime, &runtime, &priority);
        processes[i] = _createProcess(id,arrivaltime,runtime,priority);
    }
<<<<<<< Updated upstream

    
=======
    fclose(file);
>>>>>>> Stashed changes
}

Process _createProcess(int id, int arrivalTime, int runTime, int priority)
{
    Process p;
    p.id = id;
    p.arrivalTime = arrivalTime;
    p.runTime = runTime;
    p.executionTime = 0;
    p.remainingTime = runTime;
    p.priority = priority;
    return p;
}