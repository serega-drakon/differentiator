#ifndef DIFFERENTIATOR_NODE_H
#define DIFFERENTIATOR_NODE_H

#include <stdio.h>

enum Types_{
    Empty,
    Sum,
    Sub,
    UnaryPlus,
    UnaryMinus,
    Mul,
    Div,
    Pwr,
    Sin,
    Cos,
    Ln,
    Var,    ///< value -> str
    Num,    ///< value -> int / double
    Error   ///< value -> str
};

typedef struct Node_ {
    unsigned type;
    unsigned value; //FIXME: replace with void*
    void* ptrValue;
    struct Node_* prev;
    struct Node_* left;
    struct Node_* right;
} Node;

Node* nodeInit();
void nodeFree(Node* node);
void nodeClear(Node* node);
void nodeSaveToFile(FILE* output, Node* node);

#endif //DIFFERENTIATOR_NODE_H
