#include <stdio.h>      //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef short bool;
#define true 1
#define false 0

#define SHKEY 300


///==============================
//don't mess with this variable//
int * shmaddr;                 //
//===============================



int getClk()
{
    return *shmaddr;
}


/*
 * All process call this function at the beginning to establish communication between them and the clock module.
 * Again, remember that the clock is only emulation!
*/
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        //Make sure that the clock exists
        printf("Wait! The clock not initialized yet!\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *) shmat(shmid, (void *)0, 0);
}


/*
 * All process call this function at the end to release the communication
 * resources between them and the clock module.
 * Again, Remember that the clock is only emulation!
 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
 *                      It terminates the whole system and releases resources.
*/

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}


int getMessageQueueID(char* filename , char code){
    int messageQueueID = msgget(ftok(filename,code),0666|IPC_CREAT);
    if(messageQueueID == -1){
        perror("Can't create Message Queue\n");
        exit(-1);
    }
    return messageQueueID;
}

int getSharedMemory(char* filename , char code){
    int sharedMemoryID = shmget(ftok(filename,code),4,0666|IPC_CREAT);
    if((long)sharedMemoryID == -1){
        perror("Didn't create shared Memory with error\n");
    }
    return sharedMemoryID;
}

void* getSharedMemoryAddress(int sharedMemoryID){
    void* sharedMemoryAddress = shmat(sharedMemoryID,(void *)0, 0);
    return sharedMemoryAddress;
}

void destroySharedMemory(int sharedMemoryID){
    shmctl(sharedMemoryID, IPC_RMID, (struct shmid_ds *)0);
}