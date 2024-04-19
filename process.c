#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char * argv[])
{
    initClk();
    
    //TODO it needs to get the remaining time from somewhere
    remainingtime = atoi(argv[1]);
    while (remainingtime > 0)
    {
        pause(); //waiting for the signal from the parent (scheduler) to decrement the remaining time
        remainingtime--;
    }
    
    destroyClk(false);
    
    return 0;
}
