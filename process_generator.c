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
int processesCount = -1;
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
    readInputFile();
    //! testing reading the file
    // for (int i = 0; i < processesCount; i++)
    // {
    //     printf("Process %d: id = %d, arrivalTime = %d, runTime = %d, priority = %d\n", i, processes[i].id, processes[i].arrivalTime, processes[i].runTime, processes[i].priority);
    // }
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int algorithm, parameter = -1;
    getUserInput(&algorithm, &parameter);
    // 3. Initiate and create the scheduler and clock processes.
    createClock();

    createScheduler(algorithm, parameter);

    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    key_t key_id;

    key_id = ftok("pgen_sch_keyfile", 65);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);

    if (msgq_id == -1)
    {
        perror("Error in creating the queue");
        exit(-1);
    }

    printf("Message queue id = %d\n", msgq_id);
    fflush(stdout);

    int ptr = 0;
    while (ptr < processesCount)
    {
        //! ************************************************************************************************
        //! ************************************************************************************************
        //! **********************Don't forget to handle processes arriving at the same time****************
        //! ************************************************************************************************
        //! ************************************************************************************************
        if (getClk() == processes[ptr].arrivalTime)
        {
            sendProcessToScheduler(processes[ptr]);
            ptr++;
            printf("pointer is %d\n", ptr);
            fflush(stdout);
        }
    }
    printf("all processes sent\n");

    int statlock;
    int cid = waitpid(schedPid, &statlock, 0);
    if (WIFEXITED(statlock))
        printf("Scheduler terminated successfully with status %d.", WEXITSTATUS(statlock));
    else
        printf("Something went wrong with the scheduler.\n");
    
    fflush(stdout);

    // 7. Clear clock resources
    destroyClk(true);

    // 8. Clear message queue resources
    clearResources(0);
    return 0;
}

void clearResources(int signum)
{
    // TODO Clears all resources in case of interruption
    struct msqid_ds ctl_statud_ds;
    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *)0);
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
    sendval = msgsnd(msgq_id, &message, sizeof(p), !IPC_NOWAIT);
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
        perror("Error in forking a process!");
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
        execl("./clk.out", "clk", NULL);
    }
    else
    {
    }
}

void readInputFile()
{
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL)
    {
        perror("Error in opening the file");
        exit(-1);
    }
    // loop to know the number of lines
    char ch;
    while (!feof(file))
    {
        // ignore line starting with #
        fscanf(file, "%c", &ch);
        if (ch == '#')
        {
            while (ch != '\n')
                fscanf(file, "%c", &ch);
            continue;
        }
        if (ch == '\n')
            processesCount++;
    }
    rewind(file);
    processes = (Process *)malloc(processesCount * sizeof(Process));
    int id, arrivalTime, runTime, priority;
    for (int i = 0; i < processesCount; i++)
    {
        fscanf(file, "%c", &ch);
        if (ch == '#')
        {
            while (ch != '\n')
                fscanf(file, "%c", &ch);
        }
        else
            ungetc(ch, file);
        fscanf(file, "%d %d %d %d", &id, &arrivalTime, &runTime, &priority);
        processes[i] = _createProcess(id, arrivalTime, runTime, priority);
    }
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
    p.priority = priority;
    return p;
}