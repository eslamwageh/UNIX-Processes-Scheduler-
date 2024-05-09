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
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef enum
{
    FREE,
    ALLOCATED
} State;

// ! SUGGESTION: make this a black box that provides the scheduler with apis to interact with the tree without dealing with nodes and pointers
// ! SUGGESTION: make this a black box that provides the scheduler with apis to interact with the tree without dealing with nodes and pointers
// ! SUGGESTION: make this a black box that provides the scheduler with apis to interact with the tree without dealing with nodes and pointers
// ! SUGGESTION: make this a black box that provides the scheduler with apis to interact with the tree without dealing with nodes and pointers
// ! SUGGESTION: make this a black box that provides the scheduler with apis to interact with the tree without dealing with nodes and pointers

Node* createNode(int,int);

void insertProcess(Node*,Node*); // make a wrapper function that takes id and value and creates a node and calls insertProcess

void deleteProcess(Node*, int); // make a wrapper function that takes id and calls deleteProcess

//Node* root = NULL;
