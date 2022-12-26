#include "Node.h"
#include "../InputOutput/InputOutput.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

Node* nodeInit(){

    Node* node = malloc(sizeof(Node));
    node->type = Empty;
    node->value = 0; //FIXME: тут токо целые пока
    node->ptrValue = NULL;
    node->prev = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/// удаляет один узел дерева
void nodeFree(Node* node){

    free(node->ptrValue);
    free(node);
}

/// удаляет данное дерево и все его поддеревья
void nodeClear(Node* node){
    //FIXME
}

void nodePushMessage(FILE* output, Node* node){
    assert(output != NULL && node != NULL);

    fprintf(output, "\"");
    switch(node->type){
        case Empty:
            fprintf(output, "*Empty*");
            break;
        case Sum: case UnaryPlus:
            fprintf(output, "+");
            break;
        case Sub: case UnaryMinus:
            fprintf(output, "-");
            break;
        case Mul:
            fprintf(output, "*");
            break;
        case Div:
            fprintf(output, "/");
            break;
        case Pwr:
            fprintf(output, "^");
            break;
        case Sin:
            fprintf(output, "Sin");
            break;
        case Cos:
            fprintf(output, "Cos");
            break;
        case Ln:
            fprintf(output, "Ln");
            break;
        case Var:
            fprintIntS(output, (int*)node->ptrValue);
            break;
        case Num:   //FIXME: int to double or together
            fprintf(output, "%g", *(double*)node->ptrValue);
            break;
        case Error:
            fprintf(output, "*Here error*");
            break;
    }
    fprintf(output, "\"");
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
void nodePushToFile(FILE* output, Node* node){
    assert(output != NULL && node != NULL);

    fprintf(output, "%lld [label = ", (unsigned long long)node);
    nodePushMessage(output, node);
    fprintf(output,"]\n");

    if(node->left != NULL)
        fprintf(output, "%lld -> %lld\n", (unsigned long long)node, (unsigned long long)node->left);

    if(node->right != NULL)
        fprintf(output, "%lld -> %lld\n", (unsigned long long)node, (unsigned long long)node->right);

    if(node->left != NULL)
        nodePushToFile(output, node->left);

    if(node->right != NULL)
        nodePushToFile(output, node->right);
}
#pragma clang diagnostic pop

void nodeSaveToFile(FILE* output, Node* node){

    if(output != NULL){
        fprintf(output, "digraph Cock{\nnode[color=blue,fontsize=14, style=filled,fillcolor=lightgrey]\n");

        nodePushToFile(output, node);

        fprintf(output, "}\n");
    }
    else
        printf("save to file fail\n");
}
