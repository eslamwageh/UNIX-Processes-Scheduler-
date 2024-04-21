#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
bool var;

void start(int signum);
void stop(int signum);
int main(int agrc, char * argv[])
{
    initClk();
    signal(SIGUSR1, start);
    signal(SIGUSR2, stop);
    //TODO it needs to get the remaining time from somewhere
    remainingtime = atoi(argv[1]);
    int time = -1; 
    int temp;
    var = true;
    while (remainingtime > 0)
    {
        temp = getClk();
        if(var && temp != time)
        {
            time = temp;
            remainingtime--;
        }
    }
    
    destroyClk(false);
    
    return 0;
}

void start(int signum)
{
    var = true;
}

void stop(int signum)
{
    var = false;
}