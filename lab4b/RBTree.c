#include "RBTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define BLACK   "\033[2;30m"

#define WIN 1  //1 - если windows, 0 - если linux



typedef struct Info{
    char* info;
}Info;


typedef struct Node{
    int key;
    struct Node *left, *right, *parent;
    short int color;        // 0 - black, 1 - red
    Info* info;
}Node;

typedef enum ans{
    ST_OK = 0,
    KEY_AE = -1
}ans;

// ЕЛИСТ
Node EList = {0, NULL, NULL, NULL, 0, NULL};

Node* treeFind(Node* root, int key){
//    Поиск по ключу
    while (root && (root != &EList)){
        if (root->key > key){
            root = root->left;
        }
        else if (root->key < key){
            root = root->right;
        }
        else return root;
    }
    return NULL;
}

void rightRotate(Node* element, Node** root){
    Node* left = element->left;
    element->left = left->right;
    if (left->right != &EList) left->right->parent = element;
    left->parent = element->parent;
    left->right = element;
    if (left->parent == &EList) *root = left;
    else {
        if (element->parent->right == element){
            element->parent->right = left;
            element->parent = left;
        }
        else {
            element->parent->left = left;
            element->parent = left;
        }
    }
}

void leftRotate(Node* element, Node** root){
    Node* right = element->right;
    element->right = right->left;
    if (right->left != &EList) right->left->parent = element;
    right->parent = element->parent;
    right->left = element;
    if (right->parent == &EList) *root = right;
    else {
        if (element->parent->right == element){
            element->parent->right = right;
            element->parent = right;
        }
        else {
            element->parent->left = right;
            element->parent = right;
        }
    }
}

Node* getNode(int key, char* infoStr){
//    Выделение памяти под узел и инициализация
    Node* resNode = (Node*)calloc(1, sizeof(Node));
    resNode->right = &EList;
    resNode->left = &EList;
    resNode->parent = &EList;
    resNode->key = key;
    resNode->color = 1;
    resNode->info = (Info*)calloc(1, sizeof(Info));
    resNode->info->info = (char*)calloc(strlen(infoStr) + 1, sizeof(char));
    strcpy(resNode->info->info, infoStr);
    return resNode;
}



//0 - black, 1 - red
int rbInsertFix(Node* x, Node** root){
    while ((x->parent != &EList) && (x->parent->color == 1)) {
        Node* p1 = x->parent;
        Node* pp = x->parent->parent;
        // p1 в левом поддереве?
        int flag = pp->left == p1;              // для p1 в левом поддереве
        Node* p2 = NULL;
        if (flag) {
            p2 = pp->right;
        }
        else {
            p2 = pp->left;
        }
        if (p2->color == 1){                    // случай 1
            p1->color = 0;
            p2->color = 0;
            pp->color = 1;
            x = pp;
            continue;
        }
        else {
            if (flag && p1->right == x) {                                  // Случай 2 (из 2 -> 3)
                leftRotate(p1, root);
            }
            else if (!flag && p1->left == x){
                rightRotate(p1, root);
            }
            x->color = 0;                                       // Случай 3
            pp->color = 1;
            if (flag) rightRotate(pp, root);
            else leftRotate(pp, root);
            break;
        }
    }
    (*root)->parent = 0;
return 0;
}





int treeAdd(Node** root, int key, char* infoStr){
    Node* iterEL = *root;
    Node* par = NULL;
    if (!(*root)){                      // Если дерево пустое
        *root = getNode(key, infoStr);
        (*root)->color = 0;
        return ST_OK;
    }

    while (iterEL != &EList){                       // Поиск места для вставки
        par = iterEL;
        if (iterEL->key == key) return KEY_AE;
        if (iterEL->key > key) {
            iterEL = iterEL->left;
        }
        else {
            iterEL = iterEL->right;
        }
    }
    if (par->key > key){                        // Вставка
        par->left = getNode(key, infoStr);
        par->left->parent = par;
        rbInsertFix(par->left, root);
    }
    else {
        par->right = getNode(key, infoStr);
        par->right->parent = par;
        rbInsertFix(par->right, root);
    }
    return ST_OK;
}



void treeDelete();



void treePrintLine(Node* root){
//    Вывод в прямом порядке следования
    if (!root || root == &EList) return;
    printf("%d  %s\n", root->key, root->info->info);
    treePrintLine(root->left);
    treePrintLine(root->right);
}



void treePrint(Node* root, int lvl){
//    Вывод в консоль в виде дерева, изначально lvl = 0 (дерево перевернуто на pi/2 влево)
    if (root && (root != &EList)){
        treePrint(root->right, lvl + 1);
        for (int i = 0; i < lvl; i++){
            printf("   ");
        }
        if (root->color == 1){
            printf("%s%d%s\n",RED, root->key, RESET);
        }
        else {
            printf("%s%d%s\n", BLACK, root->key, RESET);
        }
        treePrint(root->left, lvl + 1);
    }
}


//void treePrintV2();

Node* treeFindMax(Node* root){
//    Поиск элемента с максимальным ключом
    while (root && (root->right != &EList)){
        root = root->right;
    }
    return root;
}

void treeFree(Node* root){
    if (root && (root != &EList)){
        treeFree(root->left);
        treeFree(root->right);
        if (root->info) {
            if (root->info->info) free(root->info->info);
            free(root->info);
        }
        free(root);
    }
}


int main() {
    Node *root = NULL;
    treeAdd(&root, 1, "a");
    treeAdd(&root, 2, "a");
    treeAdd(&root, 3, "a");
//    treeAdd(&root, 4, "a");
//    treeAdd(&root, 5, "a");
    treePrint(root, 0);
//    rightRotate(root->right->right, &root);
//    leftRotate(root->right, &root);
//    treePrint(root, 0);
    treeFree(root);
}




