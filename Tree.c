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

void insertProcess (Node* root,Node* node){
    if(inserted){
        return;
    }
    if(root == NULL){
        root = node;
        return;
    }
    
    if(node->ceiledValue == root->ceiledValue){
        if(root->id == -1){
            root->id = node->id;
            root->value = node->value;
            inserted = true;
            return;
        }
        else{
            return;
        }
    }else if(node->ceiledValue < root->ceiledValue){
        if(root->left == NULL){
            Node* newNode = createNode(root->ceiledValue/2,-1);
            newNode->parent = root;
            root->left = newNode;
            Node* rightNode = createNode(root->ceiledValue/2,-1);
            rightNode->parent = root;
            root->right = rightNode;
        }
    }

    insertProcess(root->left,node);
    insertProcess(root->right,node);
    

}


int main(){
    printf("%d\n",ceilLog2(65));
    return 0;
}
