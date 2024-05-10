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

typedef Node *MemoryHead;

typedef enum
{
    FREE,
    ALLOCATED
} State;

Node *initMemory();
int ceilLog2(int n);
void writeToMemoryLog(Node *node, FILE *file, int state, int time);
Node *createNode(int memoryValue, int id);
bool insertProcessMemory(Node *root, Node *node);
void deleteProcessMemory(Node *root, int id);
bool insertProcessWrapper(Node *, FILE *file, int, int, int);
void deleteProcessWrapper(Node *, FILE *file, int, int);
void printSpaces(int count);
void print2D(struct Node *root, int space);

int ceilLog2(int n)
{
    int count = 0;
    while (n > 1)
    {
        n = (n + 1) / 2;
        count++;
    }
    return 1 << count;
}

void writeToMemoryLog(Node *node, FILE *file, int state, int time)
{
    if (state == ALLOCATED)
    {
        fprintf(file, "At time %d allocated %d bytes for process %d from %d to %d\n", time, node->value, node->id, node->start, node->end);
    }
    else
    {
        fprintf(file, "At time %d freed %d bytes from process %d from %d to %d\n", time, node->value, node->id, node->start, node->end);
    }
}

Node *createNode(int memoryValue, int id)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->value = memoryValue;
    node->ceiledValue = ceilLog2(memoryValue);
    node->id = id;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

// 1. at the start you have only root node with value 1024 and id -1
// 2. when you insert a new process you have to check if the cield value of the process is equal to the cield value of the root node
// 3. if it is equal then you have to check if the id of the root node is -1 or not
// 3.1 if it is -1 then you have to set the id of the root node to the id of the process
// 4. if the cield value of the process is less than the value of the root node then you have to check if the left child of the root node is NULL or not
// 4.1 if it is NULL then you have to create a new node with the value of the process and set it as the left child of the root node and also set the right child of the root node to the value of the root node divided by 2
// 4.2 if it is not NULL then make a recursive call to the left child of the root node
bool inserted = false;
bool deleted = false;
int currentTime = 0;
FILE *currentFile = NULL;

bool insertProcessMemory(Node *root, Node *node)
{
    if (root == NULL)
    {
        root = node;
        // printf("Root is NULL\n");
        return false;
    }

    if (inserted || node->ceiledValue > root->ceiledValue)
    {
        // printf("Inserted or Ceiled Value is greater, current node id: %d\n",node->id);
        return false;
    }

    if (node->ceiledValue == root->ceiledValue)
    {
        if (root->id == -1 && root->left == NULL && root->right == NULL)
        {
            // printf("Root id is -1, current node id: %d\n",node->id);
            root->id = node->id;
            root->value = node->value;
            inserted = true;
            node->start = root->start;
            node->end = root->end;
            return true;
        }
        else
        {
            // printf("Root id is not -1, current node id: %d\n",node->id);
            return false;
        }
    }
    else if (node->ceiledValue < root->ceiledValue)
    {
        if (root->id == -1 && root->left == NULL)
        {
            // printf("Root has no children, current node id: %d\n",node->id);
            Node *newNode = createNode(root->ceiledValue / 2, -1);
            newNode->parent = root;
            newNode->start = root->start;
            newNode->end = root->start + newNode->ceiledValue - 1;
            root->left = newNode;
            Node *rightNode = createNode(root->ceiledValue / 2, -1);
            rightNode->parent = root;
            rightNode->start = newNode->end + 1;
            rightNode->end = rightNode->start + rightNode->ceiledValue - 1;
            root->right = rightNode;
        }
    }
    // printf("Making recursive call, current node id: %d\n",node->id);
    // printf("Root value: %d\n",root->value);
    return insertProcessMemory(root->left, node) || insertProcessMemory(root->right, node);
}

// 1. traverse the tree to find the node with the specified ID.
// 2. once the node is found:
// 2.1. if both of its children are empty delete the node along with its sibling.
// 2.2. if the children are not empty, mark the node as deleted by setting its ID to -1 and its value to half of its parent's ceiling value.
void deleteProcessMemory(Node *root, int id)
{
    if (root == NULL)
    {
        return;
    }
    if (root->id == id)
    {
        // if the current node's ID matches the target ID, it's the node to delete.
        printf("Found the node with id: %d\n", id);
        writeToMemoryLog(root, currentFile, FREE, currentTime);
        root->id = -1;                               // resetting the ID to -1 to mark it as deleted.
        root->value = root->parent->ceiledValue / 2; // resetting value to half of parent's ceiling value.
        return;                                      // exit the function after deleting the node.
    }
    deleteProcessMemory(root->left, id);
    deleteProcessMemory(root->right, id);
    // if the node was already deleted during recursive calls, no further action is needed.
    if (deleted)
    {
        return;
    }
    // if both children of the current node are empty, delete them and mark the current node as deleted.
    if (root->left && root->right && root->left->id == -1 && root->right->id == -1)
    {
        free(root->left);
        free(root->right);
        root->id = -1;
        root->right = NULL;
        root->left = NULL;
        return;
    }
    // the first time we find a node that has a child we set deleted to true to stop the deletion.
    // we have to make sure that the node has children and we stopped because of them being busy not because they don't exist.
    if (root->left && root->right)
    {
        deleted = true;
    }
}

void printSpaces(int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("  ");
    }
}

void print2D(struct Node *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    print2D(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d\n", root->value);

    // Process left child
    print2D(root->left, space);
}

bool insertProcessWrapper(Node *root, FILE *file, int id, int memSize, int time)
{
    bool good = false;
    Node *newNode = createNode(memSize, id);
    good = insertProcessMemory(root, newNode);
    inserted = false;
    if (good)
        writeToMemoryLog(newNode, file, ALLOCATED, time);
    free(newNode);
    return good;
}

void deleteProcessWrapper(Node *root, FILE *file, int id, int time)
{
    currentTime = time;
    currentFile = file;
    deleteProcessMemory(root, id);
    deleted = false;
}

Node *initMemory()
{
    Node *root = createNode(MAX_MEMORY, -1);
    insertProcessMemory(NULL, root);
    return root;
}
