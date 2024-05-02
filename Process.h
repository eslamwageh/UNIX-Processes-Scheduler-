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
  int state; // 0: started, 1: stopped, 2: resumed, 3: finished
} Process;

typedef enum Algorithm
{
  HPF_Algorithm = 1,
  SRTN_Algorithm,
  RR_Algorithm
} Algorithm;

typedef enum ProcessState{
  STARTED,
  STOPPED,
  RESUMED,
  FINISHED
} ProcessState;

#endif /* PROCESS_H */