
#include <stdlib.h>
#include <unistd.h>
#define MIN_HEAP_INITIAL_CAPACITY 1
#include "Process.h"

typedef Process *dynamicProcess;

typedef struct MinHeap
{
    /* data */
    dynamicProcess *arr;
    int size;
    int capacity;
    Algorithm algorithm;
} MinHeap;

typedef MinHeap priority_queue;

void swap(dynamicProcess *a, dynamicProcess *b)
{
    dynamicProcess temp = *a;
    *a = *b;
    *b = temp;
}

void extendCapacity(MinHeap *heap)
{
    heap->capacity *= 2;
    dynamicProcess *newArr = (dynamicProcess *)malloc(heap->capacity * sizeof(dynamicProcess));
    for (int i = 0; i < heap->size; i++)
        newArr[i] = heap->arr[i];
    free(heap->arr);
    heap->arr = newArr;
}
int getRightChild(MinHeap *heap, int index)
{
    if (2 * (index + 1) >= heap->size)
        return -1;
    return 2 * (index + 1);
}

int getLeftChild(MinHeap *heap, int index)
{
    if (2 * index + 1 >= heap->size)
        return -1;
    return 2 * index + 1;
}

int getParent(MinHeap *heap, int index)
{
    if (index <= 0)
        return -1;
    return (index - 1) / 2;
}

void heapifyUp(MinHeap *heap, int index)
{
    int parentIndex = getParent(heap, index);
    if (index == 0)
    {
        return; // If index is 0, return immediately
    }

    // For SRTN_Algorithm:
    if (heap->algorithm == SRTN_Algorithm &&
        (heap->arr[parentIndex]->remainingTime < heap->arr[index]->remainingTime ||
         (heap->arr[parentIndex]->remainingTime == heap->arr[index]->remainingTime &&
          heap->arr[parentIndex]->id < heap->arr[index]->id)))
    {
        return; // Conditions satisfied, return
    }

    // For HPF_Algorithm:
    if (heap->algorithm == HPF_Algorithm &&
        (heap->arr[parentIndex]->priority < heap->arr[index]->priority ||
         (heap->arr[parentIndex]->priority == heap->arr[index]->priority &&
          heap->arr[parentIndex]->id < heap->arr[index]->id)))
    {
        return; // Conditions satisfied, return
    }
    swap(&heap->arr[parentIndex], &heap->arr[index]);
    heapifyUp(heap, parentIndex);
}

void heapifyDown(MinHeap *heap, int index)
{
    int leftChild = getLeftChild(heap, index);
    int min = index;
    if (leftChild == -1)
        return;
    if ((heap->algorithm == SRTN_Algorithm && (heap->arr[index]->remainingTime > heap->arr[leftChild]->remainingTime || heap->arr[index]->remainingTime == heap->arr[leftChild]->remainingTime && heap->arr[index]->id > heap->arr[leftChild]->id)) || (heap->algorithm == HPF_Algorithm && (heap->arr[index]->priority > heap->arr[leftChild]->priority || heap->arr[index]->priority == heap->arr[leftChild]->priority && heap->arr[index]->id > heap->arr[leftChild]->id)))
        min = leftChild;
    int rightChild = getRightChild(heap, index);
    if ((rightChild != -1) && ((heap->algorithm == SRTN_Algorithm && (heap->arr[index]->remainingTime > heap->arr[rightChild]->remainingTime || heap->arr[index]->remainingTime == heap->arr[rightChild]->remainingTime && heap->arr[index]->id > heap->arr[rightChild]->id)) || (heap->algorithm == HPF_Algorithm && (heap->arr[index]->priority > heap->arr[rightChild]->priority || heap->arr[index]->priority == heap->arr[rightChild]->priority && heap->arr[index]->id > heap->arr[rightChild]->id))))
        min = rightChild;
    if (min == index)
        return;
    swap(&heap->arr[min], &heap->arr[index]);
    heapifyDown(heap, min);
}

MinHeap *createMinHeap(Algorithm algo)
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->size = 0;
    heap->capacity = MIN_HEAP_INITIAL_CAPACITY;
    int initialSize = heap->size + heap->capacity;
    dynamicProcess *arr = (dynamicProcess *)malloc(initialSize * sizeof(dynamicProcess));
    heap->arr = arr;
    heap->algorithm = algo;
    return heap;
}

void pushInHeap(MinHeap *heap, dynamicProcess element)
{
    if (heap->size == heap->capacity)
        extendCapacity(heap);
    heap->arr[heap->size] = element;
    heapifyUp(heap, heap->size);
    heap->size++;
}

dynamicProcess popFromHeap(MinHeap *heap)
{
    if (heap->size == 0)
        return NULL;
    dynamicProcess min = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heapifyDown(heap, 0);
    heap->size--;
    return min;
}

int isHeapEmpty(MinHeap *heap)
{
    return (heap->size == 0);
}

void printProcessInfo(dynamicProcess p)
{
    if (p == NULL)
    {
        printf("\n");
        return;
    }
    printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
           p->remainingTime, p->runTime, p->arrivalTime, p->executionTime, p->id, p->lastStoppedTime, p->pid, p->priority, p->state, p->waitingTime);
}

void printHeap(MinHeap *heap)
{
    printf("--------------------------------------------------------------------------------\n");
    printf("REM\tRUN\tARR\tEXEC\tID\tLST\tPID\tPRI\tSTATE\tWAIT\n");
    for (int i = 0; i < heap->size; i++)
    {
        printf("%d] ", i);
        printProcessInfo(heap->arr[i]);
    }
    printf("\n--------------------------------------------------------------------------------\n");
}

void destroyHeap(MinHeap *heap)
{
    printf("Destroying heap\n");
    free(heap->arr);
    free(heap);
}

dynamicProcess getMin(MinHeap *heap)
{
    if (heap->size == 0)
        return NULL;
    return heap->arr[0];
}
