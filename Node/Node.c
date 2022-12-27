#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include "Node.h"
#include "../InputOutput/InputOutput.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

Node* nodeInit(){

    Node* node = malloc(sizeof(Node));
    if(node == NULL)
        printf("nodeInit: memory alloc error\n");
    node->type = Empty;
    node->ptrValue = NULL;
    node->prev = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* nodeInitType(unsigned type){

    Node* node = nodeInit();
    if(node == NULL) return NULL;
    node->type = type;
    switch(type){
        case Var:
            node->ptrValue = malloc(sizeof(int) * MAXVAR);

            if(node->ptrValue ==  NULL){
                printf("nodeInitTypePrev: memory alloc error\n");
                free(node);
                return NULL;
            }
            break;
        case Num:
            node->ptrValue = calloc(1, sizeof(double));

            if(node->ptrValue ==  NULL){
                printf("nodeInitTypePrev: memory alloc error\n");
                free(node);
                return NULL;
            }
            break;
        default:
            break;
    }
    return node;
}

/// инициализирует узел данного типа, при необходимости выделяет память под ptrValue
Node* nodeInitTypePrev(unsigned type, Node* prevNode){

    Node* node = nodeInitType(type);
    node->prev = prevNode;
    return node;
}

/// удаляет один узел дерева
void nodeFree(Node* node){

    free(node->ptrValue);
    free(node);
}

void nodeClearRec(Node* node){

    if(node->left != NULL)
        nodeClear(node->left);
    if(node->right != NULL)
        nodeClear(node->right);

    nodeFree(node);
}

/// удаляет данное дерево и все его поддеревья
void nodeClear(Node* node){
    if(node != NULL)
        nodeClearRec(node);
}

/// nodeCopy, но с возможностью связать полученное дерево с prevNode
Node* nodeCopyPrev(Node* node, Node* prevNode){
    assert(node != NULL);

    Node* copy = nodeCopy(node);
    copy->prev = prevNode;
    return copy;
}

void nodeValueCopy(Node* fromNode, Node* toNode){
    assert(fromNode != NULL && toNode != NULL);

    switch(fromNode->type){
        case Var:
            copyStr(fromNode->ptrValue, toNode->ptrValue);
            break;
        case Num:
            *(double*)toNode->ptrValue = *(double*)fromNode->ptrValue;
            break;
        default:
            break;
    }
}

/// Инициализирует новое дерево, совпадающее с данным (ptrValue указывает на др совпадающую величину)
Node* nodeCopy(Node* node) {
    assert(node != NULL);

    Node* copy = nodeInitTypePrev(node->type, node->prev);

    nodeValueCopy(node, copy);

    if(node->left != NULL)
        copy->left = nodeCopy(node->left);
    if(node->right != NULL)
        copy->right = nodeCopy(node->right);
    return copy;
}

Node* initNumNode(double value, Node* prevNode){
    Node* numNode = nodeInitTypePrev(Num, prevNode);
    if(numNode != NULL)
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
        case Num:
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