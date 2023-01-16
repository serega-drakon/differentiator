#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "misc-no-recursion"

#include <assert.h>
#include <math.h>
#include "opt.h"

//заметим, что тут нет ни одной инициализации нового поддерева

#define OPT_LR_TREES do{\
nodeToOpt->left = opt_(nodeToOpt->left);\
nodeToOpt->right = opt_(nodeToOpt->right);} while(0)

#define OPT_U_TREE do{\
nodeToOpt->left = opt_(nodeToOpt->left);} while(0)

#define DEBUG_CHECK_B(currType) do{ \
assert(nodeToOpt->type == currType); \
assert(nodeToOpt->left != NULL && nodeToOpt->right != NULL);\
} while(0)

#define DEBUG_CHECK_U(currType) do{ \
assert(nodeToOpt->type == currType); \
assert(nodeToOpt->left != NULL);\
} while(0)

#define DEBUG_CHECK_V(currType) do{ \
assert(nodeToOpt->type == currType); \
assert(nodeToOpt->ptrValue != NULL);\
} while(0)

Node* opt_(Node* nodeToOpt);

///Чистит дерево diffNode, но не меняет его самого, возвращает обработанную копию
Node * opt(Node* diffNode){
    if(diffNode != NULL){
        Node* diffNodeCopy = nodeCopy(diffNode);
        return opt_(diffNodeCopy);
    }
    else{
        printf("opt: error diffNode is NULL");
        return NULL;
    }
}

Node* opt_sum(Node* nodeToOpt);
Node* opt_sub(Node* nodeToOpt);
Node* opt_UPlus(Node* nodeToOpt);
Node* opt_UMinus(Node* nodeToOpt);
Node* opt_mul(Node* nodeToOpt);
Node* opt_div(Node* nodeToOpt);
Node* opt_pwr(Node* nodeToOpt);
Node* opt_sin(Node* nodeToOpt);
Node* opt_cos(Node* nodeToOpt);
Node* opt_ln(Node* nodeToOpt);
Node* opt_var(Node* nodeToOpt);
Node* opt_num(Node* nodeToOpt);

Node* opt_(Node* nodeToOpt){

    switch(nodeToOpt->type){
        case Sum:
            return opt_sum(nodeToOpt);
        case Sub:
            return opt_sub(nodeToOpt);
        case UPlus:
            return opt_UPlus(nodeToOpt);
        case UMinus:
            return opt_UMinus(nodeToOpt);
        case Mul:
            return opt_mul(nodeToOpt);
        case Div:
            return opt_div(nodeToOpt);
        case Pwr:
            return opt_pwr(nodeToOpt);
        case Sin:
            return opt_sin(nodeToOpt);
        case Cos:
            return opt_cos(nodeToOpt);
        case Ln:
            return opt_ln(nodeToOpt);
        case Var:
            return opt_var(nodeToOpt);
        case Num:
            return opt_num(nodeToOpt);
        default:
            return nodeToOpt;
    }
}

Node* opt_sum(Node* nodeToOpt) {

    DEBUG_CHECK_B(Sum);

    OPT_LR_TREES;

    Node* opt_sum_1(Node* nodeToOpt);

    return opt_sum_1(nodeToOpt);
}

// это типа сценарий обработки
// проведение суммирования
Node* opt_sum_1(Node* nodeToOpt){

    Node* opt_sum_2(Node* nodeToOpt);

    if(nodeToOpt->left->type == Num && nodeToOpt->right->type == Num){

        Node* mainNumNode = nodeToOpt->left;

        double value = nGetNum(nodeToOpt->left) + nGetNum(nodeToOpt->right);
        nPushNum(mainNumNode, value);

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainNumNode);
    }
    else
        return opt_sum_2(nodeToOpt);
}

// убирание нулей
Node* opt_sum_2(Node* nodeToOpt){

    Node* opt_sum_3(Node* nodeToOpt);

    if(nodeToOpt->right->type == Num && nGetNum(nodeToOpt->right) == 0.0){

        Node* mainNode = nodeToOpt->left;

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainNode);
    }
    else if(nodeToOpt->left->type == Num && nGetNum(nodeToOpt->left) == 0.0){

        Node* mainNode = nodeToOpt->right;

        nodeFree(nodeToOpt->left);
        nodeFree(nodeToOpt);

        return opt_(mainNode);
    }
    else
        return opt_sum_3(nodeToOpt);
}

// раскрытие скобок с Sum и Sub
Node* opt_sum_3(Node* nodeToOpt){

    Node* opt_sum_4(Node* nodeToOpt);

    if(nodeToOpt->right->type == Sum || nodeToOpt->right->type == Sub){
        Node* mainNode = nodeToOpt->right;
        Node* sumNode = nodeToOpt;

        sumNode->right = mainNode->left;
        mainNode->left = sumNode;

        return opt_(mainNode);
    }
    else
        return opt_sum_4(nodeToOpt);
}

// раскрытие скобок с унарным минусом
Node* opt_sum_4(Node* nodeToOpt){

    if(nodeToOpt->right->type == UMinus){

        Node* mainSubNode = nodeToOpt;
        mainSubNode->type = Sub;

        Node* unaryMinusNode = nodeToOpt->right;

        mainSubNode->right = unaryMinusNode->left;
        nodeFree(unaryMinusNode);

        return opt_(mainSubNode);
    }
    else
        return nodeToOpt;
}


Node* opt_sub(Node* nodeToOpt) {

    DEBUG_CHECK_B(Sub);

    OPT_LR_TREES;

    Node* opt_sub_1(Node* nodeToOpt);

    return opt_sub_1(nodeToOpt);
}

Node* opt_sub_1(Node* nodeToOpt){

    Node* opt_sub_2(Node* nodeToOpt);

    if(nodeToOpt->left->type == Num && nodeToOpt->right->type == Num){

        Node* mainNumNode = nodeToOpt->left;

        double value = nGetNum(nodeToOpt->left) - nGetNum(nodeToOpt->right);
        nPushNum(mainNumNode, value);

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainNumNode);
    }
    else
        return opt_sub_2(nodeToOpt);
}

Node* opt_sub_2(Node* nodeToOpt){

    Node* opt_sub_3(Node* nodeToOpt);

    if(nodeToOpt->right->type == Num && nGetNum(nodeToOpt->right) == 0.0){

        Node* mainNode = nodeToOpt->left;

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainNode);
    }
    else if(nodeToOpt->left->type == Num && nGetNum(nodeToOpt->left) == 0.0){

        Node* node = nodeToOpt->right;

        nodeFree(nodeToOpt->left);
        nodeFree(nodeToOpt);

        Node* mainUMinusNode = nodeInitType(UMinus);
        mainUMinusNode->left = node;

        return opt_(mainUMinusNode);
    }
    else
        return opt_sub_3(nodeToOpt);
}

// раскрытие скобок с Sum и Sub
Node* opt_sub_3(Node* nodeToOpt){

    Node* opt_sub_4(Node* nodeToOpt);

    if(nodeToOpt->right->type == Sum){

        Node* mainSubNode = nodeToOpt->right;
        mainSubNode->type = Sub;

        Node* subNode = nodeToOpt;

        subNode->right = mainSubNode->left;
        mainSubNode->left = subNode;

        return opt_(mainSubNode);
    }
    else if(nodeToOpt->right->type == Sub){

        Node* mainSumNode = nodeToOpt->right;
        mainSumNode->type = Sum;

        Node* subNode = nodeToOpt;

        subNode->right = mainSumNode->left;
        mainSumNode->left = subNode;

        return opt_(mainSumNode);
    }
    else
        return opt_sub_4(nodeToOpt);
}

Node* opt_sub_4(Node* nodeToOpt){

    if(nodeToOpt->right->type == UMinus){

        Node* mainSumNode = nodeToOpt;
        mainSumNode->type = Sum;

        Node* unaryMinusNode = nodeToOpt->right;

        mainSumNode->right = unaryMinusNode->left;
        nodeFree(unaryMinusNode);

        return opt_(mainSumNode);
    }
    else
        return nodeToOpt;
}

// унарный плюс бесполезен, его я просто убираю
Node* opt_UPlus(Node* nodeToOpt) {

    DEBUG_CHECK_U(UPlus);

    return opt_(nodeToOpt->left);
}

Node* opt_UMinus(Node* nodeToOpt) {

    DEBUG_CHECK_U(UMinus);

    OPT_U_TREE;

    Node* opt_UMinus_1(Node* nodeToOpt);

    return opt_UMinus_1(nodeToOpt);
}

Node* opt_UMinus_1(Node* nodeToOpt) {

    if(nodeToOpt->left->type == Num && nGetNum(nodeToOpt->left) == 0){
        Node* numNode = nodeToOpt->left;
        nodeFree(nodeToOpt);
        return numNode;
    }
    else
        return nodeToOpt;
}


Node* opt_mul(Node* nodeToOpt) { //var * 1 - 1 0

    DEBUG_CHECK_B(Mul);

    OPT_LR_TREES;

    Node* opt_mul_1(Node* nodeToOpt);

    return opt_mul_1(nodeToOpt);
}

Node* opt_mul_1(Node* nodeToOpt){

    Node* opt_mul_2(Node* nodeToOpt);

    if(nodeToOpt->left->type == Num && nodeToOpt->right->type == Num){

        Node* mainNumNode = nodeToOpt->left;

        double value = nGetNum(nodeToOpt->left) * nGetNum(nodeToOpt->right);
        nPushNum(mainNumNode, value);

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainNumNode);
    }
    else
        return opt_mul_2(nodeToOpt);
}

Node* opt_mul_3(Node* nodeToOpt);

Node* opt_mul_2(Node* nodeToOpt){

    Node* opt_mul_2_RNum(Node* nodeToOpt);
    Node* opt_mul_2_LNum(Node* nodeToOpt);

    if(nodeToOpt->right->type == Num)
        return opt_mul_2_RNum(nodeToOpt);
    else if(nodeToOpt->left->type == Num)
        return opt_mul_2_LNum(nodeToOpt);
    else
        return opt_mul_3(nodeToOpt);
}

Node* opt_mul_2_RNum(Node* nodeToOpt){
    assert(nodeToOpt->right->type == Num);

    double nodeValue = nGetNum(nodeToOpt->right);

    if(nodeValue == 0.0){

        Node* mainNumNode = nodeToOpt->right;

        nodeFree(nodeToOpt->left);
        nodeFree(nodeToOpt);

        return opt_(mainNumNode);
    }
    else if(nodeValue == 1.0){

        Node* mainVarNode = nodeToOpt->left;

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainVarNode);
    }
    else if(nodeValue == - 1.0){

        Node* mainUMinusNode = nodeInitType(UMinus);

        mainUMinusNode->left = nodeToOpt->left;

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainUMinusNode);
    }

    return opt_mul_3(nodeToOpt);
}

Node* opt_mul_2_LNum(Node* nodeToOpt) {
    assert(nodeToOpt->left->type == Num);

    double nodeValue = nGetNum(nodeToOpt->left);

    if (nodeValue == 0.0) {

        Node *mainNumNode = nodeToOpt->left;

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainNumNode);
    } else if (nodeValue == 1.0) {

        Node *mainVarNode = nodeToOpt->right;

        nodeFree(nodeToOpt->left);
        nodeFree(nodeToOpt);

        return opt_(mainVarNode);
    } else if (nodeValue == -1.0) {

        Node *mainUMinusNode = nodeInitType(UMinus);

        mainUMinusNode->left = nodeToOpt->right;

        nodeFree(nodeToOpt->left);
        nodeFree(nodeToOpt);

        return opt_(mainUMinusNode);
    }

    return opt_mul_3(nodeToOpt);
}

Node* opt_mul_3(Node* nodeToOpt){

    if(nodeToOpt->right->type == Mul){

        Node* mainMulNode = nodeToOpt->right;
        Node* mulNode = nodeToOpt;

        mulNode->right = mainMulNode->left;
        mainMulNode->left = mulNode;

        return opt_(mainMulNode);
    }
    else
        return nodeToOpt;
}

Node* opt_div(Node* nodeToOpt) {

    DEBUG_CHECK_B(Div);

    OPT_LR_TREES;

    Node* opt_div_1(Node* nodeToOpt);

    return opt_div_1(nodeToOpt);
}

Node* opt_div_1(Node* nodeToOpt){

    Node* opt_div_2(Node* nodeToOpt);

    if(nodeToOpt->left->type == Num && nodeToOpt->right->type == Num){

        Node* mainNumNode = nodeToOpt->left;

        double value = nGetNum(nodeToOpt->left) / nGetNum(nodeToOpt->right);
        nPushNum(mainNumNode, value);

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainNumNode);
    }
    else
        return opt_div_2(nodeToOpt);
}

Node* opt_div_2(Node* nodeToOpt){

    if(nodeToOpt->left->type == Num && nGetNum(nodeToOpt->left) == 0.0){

        Node* mainNumNode = nodeToOpt->left;

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainNumNode);
    }
    else
        return nodeToOpt;
}


Node* opt_pwr(Node* nodeToOpt) {

    DEBUG_CHECK_B(Pwr);

    OPT_LR_TREES;

    Node* opt_pwr_1(Node* nodeToOpt);

    return opt_pwr_1(nodeToOpt);
}

Node* opt_pwr_1(Node* nodeToOpt){

    Node* opt_pwr_2(Node* nodeToOpt);

    if(nodeToOpt->left->type == Num && nodeToOpt->right->type == Num){

        Node* mainNumNode = nodeToOpt->left;

        double value = pow(nGetNum(nodeToOpt->left), nGetNum(nodeToOpt->right));
        nPushNum(mainNumNode, value);

        nodeFree(nodeToOpt->right);
        nodeFree(nodeToOpt);

        return opt_(mainNumNode);
    }
    else
        return opt_pwr_2(nodeToOpt);
}

Node* opt_pwr_2(Node* nodeToOpt){

    Node* opt_pwr_2_1(Node* nodeToOpt);
    Node* opt_pwr_2_2(Node* nodeToOpt);
    Node* opt_pwr_2_3(Node* nodeToOpt);

    if(nodeToOpt->left->type == Num && nGetNum(nodeToOpt->left) == 1.0)
        return opt_pwr_2_1(nodeToOpt);

    else if(nodeToOpt->right->type == Num){

        double nodeValue = nGetNum(nodeToOpt->right);

        if(nodeValue == 0.0)
            return opt_pwr_2_2(nodeToOpt);
        else if(nodeValue == 1.0)
            return opt_pwr_2_3(nodeToOpt);
    }
    return nodeToOpt;
}

Node* opt_pwr_2_1(Node* nodeToOpt){
    assert(nodeToOpt->left->type == Num && nGetNum(nodeToOpt->left) == 1.0);

    Node* mainNumNode = nodeToOpt->left;

    nodeFree(nodeToOpt->right);
    nodeFree(nodeToOpt);

    return opt_(mainNumNode);
}

Node* opt_pwr_2_2(Node* nodeToOpt){
    assert(nodeToOpt->right->type == Num && nGetNum(nodeToOpt->right) == 0.0);

    Node* mainNumNode = nodeToOpt->right;

    nPushNum(mainNumNode, 1);

    nodeFree(nodeToOpt->left);
    nodeFree(nodeToOpt);

    return opt_(mainNumNode);
}

Node* opt_pwr_2_3(Node* nodeToOpt){
    assert(nodeToOpt->right->type == Num && nGetNum(nodeToOpt->right) == 1.0);

    Node* mainNode = nodeToOpt->left;

    nodeFree(nodeToOpt->right);
    nodeFree(nodeToOpt);

    return opt_(mainNode);
}

Node* opt_sin(Node* nodeToOpt) {

    DEBUG_CHECK_U(Sin);

    OPT_U_TREE;

    return nodeToOpt;
}

Node* opt_cos(Node* nodeToOpt) {

    DEBUG_CHECK_U(Cos);

    OPT_U_TREE;

    return nodeToOpt;
}

Node* opt_ln(Node* nodeToOpt) {

    DEBUG_CHECK_U(Ln);

    OPT_U_TREE;

    return nodeToOpt;
}

Node* opt_var(Node* nodeToOpt) {

    DEBUG_CHECK_V(Var);

    return nodeToOpt;
}

Node* opt_num(Node* nodeToOpt) {

    DEBUG_CHECK_V(Num);

    return nodeToOpt;
}

#pragma clang diagnostic pop