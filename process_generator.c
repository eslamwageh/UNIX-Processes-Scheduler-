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

void sendProcessToScheduler(Process p) 
{
    msgbuff message;
    message.mtype = 1;
    message.msg_process = p;
    sendval = msgsnd(msgq_id, &message, sizeof(message.msg_process), !IPC_NOWAIT);

    if (sendval == -1)
    {
        perror("Error in sending to scheduler");
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    key_t key_id;
    int sendval;

    key_id = ftok("pgen_sch_keyfile", 65);
    msgq_id = msgget(key_id, 0666 | IPC_CREAT);

    if (msgq_id == -1)
    {
        perror("Error in creating the queue");
        exit(-1);
    }
    printf("Message queue id = %d\n", msgq_id);

   
    
    
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    // TODO Clears all resources in case of interruption
    struct msqid_ds ctl_statud_ds;
    msgctl(msgq_id, IPC_RMID, (struct msqid_ds *)0);
}
