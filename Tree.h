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
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;



Node* createNode(int,int);

void insertProcess(Node*,Node*);

void deleteProcess(Node*, int);


//Node* root = NULL;
