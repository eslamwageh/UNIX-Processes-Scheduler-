#include "Tree.h"


int ceilLog2(int n) {
    int count = 0;
    while (n > 1) {
        n = (n + 1) / 2;
        count++;
    }
    return 1<<count;
}

Node* createNode (int memoryValue,int id){
    Node* node = malloc(sizeof(Node)) ;
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

void insertProcess (Node* root, Node* node){
    if(root == NULL){
        root = node;
        // printf("Root is NULL\n");
        return;
    }
    
    if(inserted || node->ceiledValue > root->ceiledValue){
        // printf("Inserted or Ceiled Value is greater, current node id: %d\n",node->id);
        return;
    }
    
    if(node->ceiledValue == root->ceiledValue){
        if(root->id == -1 && root->left == NULL && root->right == NULL){
            // printf("Root id is -1, current node id: %d\n",node->id);
            root->id = node->id;
            root->value = node->value;
            inserted = true;
            free(node);
            return;
        }
        else{
            // printf("Root id is not -1, current node id: %d\n",node->id);
            return;
        }
    }else if(node->ceiledValue < root->ceiledValue){
        if(root->id == -1 && root->left == NULL){
            // printf("Root has no children, current node id: %d\n",node->id);
            Node* newNode = createNode(root->ceiledValue/2,-1);
            newNode->parent = root;
            root->left = newNode;
            Node* rightNode = createNode(root->ceiledValue/2,-1);
            rightNode->parent = root;
            root->right = rightNode;
        }
    }
    // printf("Making recursive call, current node id: %d\n",node->id);
    // printf("Root value: %d\n",root->value);
    insertProcess(root->left,node);
    insertProcess(root->right,node);
    

}



void printSpaces(int count) {
    for (int i = 0; i < count; i++) {
        printf("  ");
    }
}

void print2D(struct Node* root, int space)
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

int main(){
    Node* root = createNode(MAX_MEMORY,-1);
    insertProcess(NULL,root);
    Node* node = createNode(7,1);
    inserted = false;
    insertProcess(root,node);
    inserted = false;
    print2D(root,0);
    Node* node2 = createNode(63,2);
    insertProcess(root,node2);
    inserted = false;
    Node* node3 = createNode(31,3);
    insertProcess(root,node3);
    inserted = false;
    Node* node4 = createNode(15,4);
    insertProcess(root,node4);
    print2D(root,0);

    //deleteProcess(root,1);
    print2D(root,0);

    return 0;
}