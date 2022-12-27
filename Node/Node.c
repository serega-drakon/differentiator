#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include "Node.h"
#include "../InputOutput/InputOutput.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

Node* nodeInit(){

    Node* node = malloc(sizeof(Node));
    node->type = Empty;
    node->ptrValue = NULL;
    node->prev = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/// инициализирует узел данного типа, при необходимости выделяет память под ptrValue
Node* nodeInitType(unsigned type, Node* prevNode){
    Node* node = nodeInit();
    node->type = type;
    node->prev = prevNode;
    switch(type){
        case Var:
            node->ptrValue = malloc(sizeof(int) * MAXVAR);
            break;
        case Num:
            node->ptrValue = calloc(1, sizeof(double));
            break;
        default:
            break;
    }
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

void* nodeValueCopy(Node* node){
    assert(node != NULL);

    switch(node->type){
        case Var:
            return initCopiedStr(node->ptrValue, MAXVAR);
        case Num:
            return initCopiedNum(node->ptrValue);
        default:
            return NULL;
    }
}

/// nodeCopy, но с возможностью связать полученное дерево с prevNode
Node* nodeCopyPrev(Node* node, Node* prevNode){
    assert(node != NULL);

    Node* copy = nodeCopy(node);
    copy->prev = prevNode;
    return copy;
}

/// Инициализирует новое дерево, совпадающее с данным (ptrValue указывает на др совпадающую величину)
Node *nodeCopy(Node *node) { //FIXME: dont forget about ptrValue!
    assert(node != NULL);

    Node* copy = nodeInit();
    copy->type = node->type;
    copy->prev = node->prev;
    copy->ptrValue = nodeValueCopy(node);
    if(node->left != NULL)
        copy->left = nodeCopy(node->left);
    if(node->right != NULL)
        copy->right = nodeCopy(node->right);
    return copy;
}

Node* initNumNode(double value, Node* prevNode){
    Node* numNode = nodeInitType(Num, prevNode);
    *(double*)numNode->ptrValue = value;
    return numNode;
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

void nodeSaveToFile(FILE* output, Node* node){

    if(output != NULL){
        fprintf(output, "digraph Cock{\nnode[color=blue,fontsize=14, style=filled,fillcolor=lightgrey]\n");

        nodePushToFile(output, node);

        fprintf(output, "}\n");
    }
    else
        printf("save to file fail\n");
}

#pragma clang diagnostic pop