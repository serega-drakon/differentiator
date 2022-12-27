#ifndef DIFFERENTIATOR_NODE_H
#define DIFFERENTIATOR_NODE_H

#include <stdio.h>

#define MEM_ERR do{printf("memory error\n"); return NULL;} while(0)

#define MEM_CHECK(var) do{if(var == NULL) MEM_ERR; } while(0)

#define MAXVAR 100

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
    void* ptrValue;
    struct Node_* prev;
    struct Node_* left;
    struct Node_* right;
} Node;

Node* nodeInit();
Node* nodeInitTypePrev(unsigned type, Node* prevNode);
Node* nodeInitType(unsigned type);
Node* initNumNode(double value, Node* prevNode);
void nodeFree(Node* node);
void nodeClear(Node* node);
void nodeSaveToFile(FILE* output, Node* node);
Node* nodeCopy(Node *node);
Node* nodeCopyPrev(Node* node, Node* prevNode);

#endif //DIFFERENTIATOR_NODE_H
