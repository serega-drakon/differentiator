#ifndef DIFFERENTIATOR_NODE_H
#define DIFFERENTIATOR_NODE_H

#include <stdio.h>

#define MEM_ERR do{printf("memory error\n"); return NULL;} while(0)

#define MEM_CHECK(var) do{if(var == NULL) MEM_ERR; } while(0)

#define MAXVAR 100

enum Types_{
    Error,
    Empty,
    Var,    ///< value -> str
    Num,    ///< value -> int / double
    Sum,
    Sub,
    UPlus,
    UMinus,
    Mul,
    Div,
    Pwr,
    Sin,
    Cos,
    Ln,
};

typedef struct Node_ {
    unsigned type;
    void* ptrValue;
    struct Node_* left;
    struct Node_* right;
} Node;

Node* nodeInit();
Node* nodeInitType(unsigned type);
Node* initNumNode(double value);
double nGetNum(Node* numNode);
double nPushNum(Node* numNode, double value);
int nPushVar(Node* varNode, int value, unsigned x);
int nGetVar(Node* varNode, unsigned x);
void nodeFree(Node* node);
void nodeClear(Node* node);
void nodeSaveToFile(FILE* output, Node* node);
Node* nodeCopy(Node *node);

#endif //DIFFERENTIATOR_NODE_H
