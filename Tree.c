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


void insertProcess (Node* root,Node* node){
    if(root == NULL){
        root = node;
        return;
    }
    
    // else if(root->value == node->ceiledValue && root->id = -1){

    // }

    insertProcess(root->left , node);

    insertProcess(root->right,node);

}


int main(){
    printf("%d\n",ceilLog2(65));
    return 0;
}
