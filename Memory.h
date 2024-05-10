#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#define MAX_MEMORY 1024

typedef struct Node
{
    /* data */
    int ceiledValue;
    int value;
    int id;
    int start;
    int end;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

typedef enum
{
    FREE,
    ALLOCATED
} State;

Node *initMemory();

void insertProcessWrapper(Node *, FILE *file, int, int, int);

void deleteProcessWrapper(Node *, FILE *file, int, int);
