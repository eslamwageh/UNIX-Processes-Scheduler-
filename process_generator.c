#include "headers.h"
#include "PriorityQueue.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgq_id1;
int msgq_id2;
void clearResources(int);

typedef struct msgbuff
{
    long mtype;
    Process msg_process;
} msgbuff;

int sendval;
int processesCount = 0;
int schedPid;
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
    char *filename = argv[1];

    printf("%s \n", argv[1]);
    printf("%s \n", argv[2]);
    printf("%s \n", argv[3]);

    readInputFile();

    //! testing reading the file
    // for (int i = 0; i < processesCount; i++)
    // {
    //     printf("Process %d: id = %d, arrivalTime = %d, runTime = %d, priority = %d\n", i, processes[i].id, processes[i].arrivalTime, processes[i].runTime, processes[i].priority);
    // }

    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int algorithm, parameter = -1;

    getUserInput(&algorithm, &parameter);
    //  3. Initiate and create the scheduler and clock processes.
    createClock();

    createScheduler(algorithm, parameter);

    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
    fflush(stdout);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.

    msgq_id1 = getMessageQueueID("pgen_sch_keyfile", 65);
    msgq_id2 = getMessageQueueID("pgen_sch_keyfile", 66);
    printf("Message queue id =    %d\n", msgq_id1);
    fflush(stdout);

    int ptr = 0;
    while (ptr < processesCount)
    {
        //! ************************************************************************************************
        //! ************************************************************************************************
        //! **********************Don't forget to handle processes arriving at the same time****************
        //! ************************************************************************************************
        //! ************************************************************************************************
        while (getClk() == processes[ptr].arrivalTime)
        {
            sendProcessToScheduler(processes[ptr]);
            ptr++;
            printf("pointer is %d\n", ptr);
            fflush(stdout);
            fflush(stdout);
        }
    }
    printf("all processes sent\n");
    fflush(stdout);
    kill(schedPid, SIGUSR1);

    int statlock;
    int cid = waitpid(schedPid, &statlock, 0);
    if (WIFEXITED(statlock))
        printf("Scheduler terminated successfully with status %d.", WEXITSTATUS(statlock));
    else
        printf("Something went wrong with the scheduler.\n");

    fflush(stdout);

    // i think clear resources should be before destroy clock as destroy clock(true)
    // terminates all the processes including process generator so it will no be able
    // to continue to clear recources
    // 7. Clear clock resources
    destroyClk(true);

    // 8. Clear message queue resources

    return 0;
}

void clearResources(int signum)
{
    printf("\n\n\n\n\n\nclearing resources\n\n\n\n\n");
    // TODO Clears all resources in case of interruption
    struct msqid_ds ctl_statud_ds;
    msgctl(msgq_id1, IPC_RMID, (struct msqid_ds *)0);
    msgctl(msgq_id2, IPC_RMID, (struct msqid_ds *)0);
    destroySharedMemory(getSharedMemory("sch_pcs_keyfile", 'A'));
    free(processes);
    // kill(-getpgrp(), SIGKILL);
}

void sendProcessToScheduler(Process p)
{
    printf("sending process to scheduler\n");
    fflush(stdout);
    msgbuff message;
    message.mtype = 1;
    message.msg_process = p;
    kill(schedPid, SIGUSR2);
    printf("sent signal to scheduler\n");
    fflush(stdout);
    sendval = msgsnd(msgq_id1, &message, sizeof(message.msg_process), !IPC_NOWAIT);
    fflush(stdout);
    if (sendval == -1)
    {
        perror("Error in sending to scheduler");
        fflush(stdout);
    }
    else
    {
        printf("sent a process to scheduler with id = %d\n", message.msg_process.id);
        fflush(stdout);
    }
    int recval = msgrcv(msgq_id2, &message, sizeof(message.msg_process), 0, !IPC_NOWAIT);
    if (recval != -1)
    {
        printf("Acknowledged\n");
        fflush(stdout);
    }
    fflush(stdout);
}

void getUserInput(int *algorithm, int *parameter)
{
    printf("Please Enter the type of the algoritm: \n1: HPF \n2: SRTN \n3: RR\n");
    fflush(stdout);
    scanf("%d", algorithm);
    if (*algorithm == 3)
    {
        printf("Please Enter the time quantum: \n");
        fflush(stdout);
        scanf("%d", parameter);
    }
}

void createScheduler(int algorithm, int parameter)
{
    int pid = fork();
    if (pid == -1)
    {
        perror("Error in forking a process!");
        fflush(stdout);
    }
    else if (pid == 0)
    {
        char arg1[10], arg2[10], arg3[10];
        sprintf(arg1, "%d", algorithm);
        sprintf(arg2, "%d", parameter);
        sprintf(arg3, "%d", processesCount);
        execl("./scheduler.out", "scheduler", arg1, arg2, arg3, NULL);
    }
    else
    {
        schedPid = pid;
        printf("sched pid is %d\n", schedPid);
        fflush(stdout);
    }
}

void createClock()
{
    printf("before clk fork\n");
    fflush(stdout);
    int pid = fork();
    if (pid == -1)
        perror("Error in forking a process!");
    else if (pid == 0)
    {
        printf("cc");
        fflush(stdout);
        execl("./clk.out", "clk", NULL);
    }
    else
    {
    }
}

void readInputFile()
{
    FILE *file = fopen("PROCESSES", "r");
    int id, arrivaltime, runtime, priority;
    char line[100];
    fgets(line, sizeof(line), file);
    while (fscanf(file, "%d\t%d\t%d\t%d\n", &id, &arrivaltime, &runtime, &priority) == 4)
        processesCount++;
    fclose(file);
    file = fopen("PROCESSES", "r");
    fgets(line, sizeof(line), file);
    processes = (Process *)malloc(processesCount * sizeof(Process));
    for (int i = 0; i < processesCount; i++)
    {
        fscanf(file, "%d %d %d %d", &id, &arrivaltime, &runtime, &priority);
        processes[i] = _createProcess(id, arrivaltime, runtime, priority);
    }
    fclose(file);
}

Process _createProcess(int id, int arrivalTime, int runTime, int priority)
{
    Process p;
    p.id = id;
    p.arrivalTime = arrivalTime;
    p.runTime = runTime;
    p.executionTime = 0;
    p.remainingTime = runTime;
    p.waitingTime = 0;
    p.state = 0;
    p.pid = -1;
    p.priority = priority;
    p.lastStoppedTime = arrivalTime;
    return p;
}