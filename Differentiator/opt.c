#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include "opt.h"
#include "../Node/Node.h"

Node* opt_(Node* nodeToOpt, Node* prevNode);

///Чистит дерево diffNode, но не меняет его самого, возвращает обработанную копию
Node * opt(Node* diffNode){
    if(diffNode != NULL){
        Node* diffNodeCopy = nodeCopy(diffNode);
        return opt_(diffNodeCopy, NULL);
    }
    else{
        printf("opt: error diffNode is NULL");
        return NULL;
    }
}

Node* opt_sum(Node* nodeToOpt, Node* prevNode);
Node* opt_sub(Node* nodeToOpt, Node* prevNode);
Node* opt_unaryPlus(Node* nodeToOpt, Node* prevNode);
Node* opt_unaryMinus(Node* nodeToOpt, Node* prevNode);
Node* opt_mul(Node* nodeToOpt, Node* prevNode);
Node* opt_div(Node* nodeToOpt, Node* prevNode);
Node* opt_pwr(Node* nodeToOpt, Node* prevNode);
Node* opt_sin(Node* nodeToOpt, Node* prevNode);
Node* opt_cos(Node* nodeToOpt, Node* prevNode);
Node* opt_ln(Node* nodeToOpt, Node* prevNode);
Node* opt_var(Node* nodeToOpt, Node* prevNode);
Node* opt_num(Node* nodeToOpt, Node* prevNode);

Node* opt_(Node* nodeToOpt, Node* prevNode){

    switch(nodeToOpt->type){
        case Sum:
            return opt_sum(nodeToOpt, prevNode);
        case Sub:
            return opt_sub(nodeToOpt, prevNode);
        case UnaryPlus:
            return opt_unaryPlus(nodeToOpt, prevNode);
        case UnaryMinus:
            return opt_unaryMinus(nodeToOpt, prevNode);
        case Mul:
            return opt_mul(nodeToOpt, prevNode);
        case Div:
            return opt_div(nodeToOpt, prevNode);
        case Pwr:
            return opt_pwr(nodeToOpt, prevNode);
        case Sin:
            return opt_sin(nodeToOpt, prevNode);
        case Cos:
            return opt_cos(nodeToOpt, prevNode);
        case Ln:
            return opt_ln(nodeToOpt, prevNode);
        case Var:
            return opt_var(nodeToOpt, prevNode);
        case Num:
            return opt_num(nodeToOpt, prevNode);
        default:
            return nodeToOpt;
    }
}

Node* opt_sum(Node* nodeToOpt, Node* prevNode) {

    
}

Node* opt_sub(Node* nodeToOpt, Node* prevNode) {}
Node* opt_unaryPlus(Node* nodeToOpt, Node* prevNode) {}
Node* opt_unaryMinus(Node* nodeToOpt, Node* prevNode) {}
Node* opt_mul(Node* nodeToOpt, Node* prevNode) {}
Node* opt_div(Node* nodeToOpt, Node* prevNode) {}
Node* opt_pwr(Node* nodeToOpt, Node* prevNode) {}
Node* opt_sin(Node* nodeToOpt, Node* prevNode) {}
Node* opt_cos(Node* nodeToOpt, Node* prevNode) {}
Node* opt_ln(Node* nodeToOpt, Node* prevNode) {}
Node* opt_var(Node* nodeToOpt, Node* prevNode) {}
Node* opt_num(Node* nodeToOpt, Node* prevNode) {}

#pragma clang diagnostic pop