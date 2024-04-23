#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char * argv[])
{
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    
    initClk();
    remainingtime = atoi(argv[1]);
    printf("remaining time = %d\n", remainingtime);
    while (remainingtime > 0)
    {
        pause();
        remainingtime--;
        printf("remaining time = %d\n", remainingtime);
    }
    
    destroyClk(false);
    return 0;
}
