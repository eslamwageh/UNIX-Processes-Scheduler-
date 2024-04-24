#ifndef PROCESS_H
#define PROCESS_H

typedef struct Process
{
    int id;
    int pid;
    int priority;
    int arrivalTime;
    int runTime;
    int executionTime;
    int remainingTime;
    int waitingTime;
    int lastStoppedTime;
    int state; //0: created, 1: stopped, 2: resumed, 3: finished
} Process;

typedef enum Algorithm {
  HPF_Algorithm,
  SRTN_Algorithm
} Algorithm;

#endif /* PROCESS_H */