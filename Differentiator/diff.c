#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include <assert.h>
#include "diff.h"
#include "../InputOutput/InputOutput.h"

Node* diff_(Node* nodeToDiff, Node* prevNode);

const int* diffVar;

Node *diff(Node *exprNode, const int var[]) {
    if(exprNode == NULL) {
        printf("diff: error NULL exprNode");
        return NULL;
    }
    diffVar = var;
    return  diff_(exprNode, NULL);
}

Node* diff_sum(Node* nodeToDiff, Node* prevNode);
Node* diff_sub(Node* nodeToDiff, Node* prevNode);
Node* diff_unaryPlus(Node* nodeToDiff, Node* prevNode);
Node* diff_unaryMinus(Node* nodeToDiff, Node* prevNode);
Node* diff_mul(Node* nodeToDiff, Node* prevNode);
Node* diff_div(Node* nodeToDiff, Node* prevNode);
Node* diff_pwr(Node* nodeToDiff, Node* prevNode);
Node* diff_sin(Node* nodeToDiff, Node* prevNode);
Node* diff_cos(Node* nodeToDiff, Node* prevNode);
Node* diff_ln(Node* nodeToDiff, Node* prevNode);
Node* diff_var(Node* nodeToDiff, Node* prevNode);
Node* diff_num(Node* nodeToDiff, Node* prevNode);

Node* diff_(Node* nodeToDiff, Node* prevNode){      //FIXME: check for errors
    switch(nodeToDiff->type){
        case Sum:
            return diff_sum(nodeToDiff, prevNode);
        case Sub:
            return diff_sub(nodeToDiff, prevNode);
        case UnaryPlus:
            return diff_unaryPlus(nodeToDiff, prevNode);
        case UnaryMinus:
            return diff_unaryMinus(nodeToDiff, prevNode);
        case Mul:
            return diff_mul(nodeToDiff, prevNode);
        case Div:
            return diff_div(nodeToDiff, prevNode);
        case Pwr:
            return diff_pwr(nodeToDiff, prevNode);
        case Sin:
            return diff_sin(nodeToDiff, prevNode);
        case Cos:
            return diff_cos(nodeToDiff, prevNode);
        case Ln:
            return diff_ln(nodeToDiff, prevNode);
        case Var:
            return diff_var(nodeToDiff, prevNode);
        case Num:
            return diff_num(nodeToDiff, prevNode);
        default:
            return nodeToDiff;
    }
}

Node* diff_sum(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Sum);
    // main
    Node* mainSumNode = nodeInitType(Sum, prevNode);
    mainSumNode->left = diff_(nodeToDiff->left, mainSumNode);
    mainSumNode->right = diff_(nodeToDiff->right, mainSumNode);
    return mainSumNode;
}

Node* diff_sub(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Sub);
    // main
    Node* mainSubNode = nodeInitType(Sub, prevNode);
    mainSubNode->left = diff_(nodeToDiff->left, mainSubNode);
    mainSubNode->right = diff_(nodeToDiff->right, mainSubNode);
    return mainSubNode;
}

Node* diff_unaryPlus(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == UnaryPlus);
    // main
    Node* mainUnaryPlusNode = nodeInitType(UnaryPlus, prevNode);
    mainUnaryPlusNode->left = diff_(nodeToDiff->left, mainUnaryPlusNode);
    return mainUnaryPlusNode;
}

Node* diff_unaryMinus(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == UnaryMinus);
    // main
    Node* mainUnaryMinusNode = nodeInitType(UnaryMinus, prevNode);
    mainUnaryMinusNode->left = diff_(nodeToDiff->left, mainUnaryMinusNode);
    return mainUnaryMinusNode;
}

Node* diff_mul(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Mul);
    // main
    Node* mainSumNode = nodeInitType(Sum, prevNode);

    Node* mulNodeLeft = nodeInitType(Mul, mainSumNode);
    mainSumNode->left = mulNodeLeft;
    mulNodeLeft->left = diff_(nodeToDiff->left, mulNodeLeft);
    mulNodeLeft->right = nodeCopyPrev(nodeToDiff->right, mulNodeLeft);

    Node* mulNodeRight = nodeInitType(Mul, mainSumNode);
    mainSumNode->right = mulNodeRight;
    mulNodeRight->left = nodeCopyPrev(nodeToDiff->left, mulNodeRight);
    mulNodeRight->right = diff_(nodeToDiff->right, mulNodeRight);

    return mainSumNode;
}

Node* diff_div(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Div);
    // main
    Node* mainDivNode = nodeInitType(Div, prevNode);

    Node* subNodeLeft = nodeInitType(Sub, mainDivNode);
    mainDivNode->left = subNodeLeft;

    Node* mulNodeLeft = nodeInitType(Mul, subNodeLeft);
    subNodeLeft->left = mulNodeLeft;
    mulNodeLeft->left = diff_(nodeToDiff->left, mulNodeLeft);
    mulNodeLeft->right = nodeCopyPrev(nodeToDiff->right, mulNodeLeft);

    Node* mulNodeRight = nodeInitType(Mul, subNodeLeft);
    subNodeLeft->right = mulNodeRight;
    mulNodeRight->left = nodeCopyPrev(nodeToDiff->left, mulNodeRight);
    mulNodeRight->right = diff_(nodeToDiff->right, mulNodeRight);

    Node* pwrNodeRight = nodeInitType(Pwr, mainDivNode);
    mainDivNode->right = pwrNodeRight;
    pwrNodeRight->left = nodeCopyPrev(nodeToDiff->right, pwrNodeRight);
    pwrNodeRight->right = initNumNode(2, pwrNodeRight);

    return mainDivNode;
}

Node* diff_pwr(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Pwr);
    // main
    Node* mainMulNode = nodeInitType(Mul, prevNode);
    // main -> left
    mainMulNode->left = nodeCopyPrev(nodeToDiff, mainMulNode);
    // main -> right
    Node* sumNodeRight = nodeInitType(Sum, mainMulNode);
    mainMulNode->right = sumNodeRight;
    // main -> right -> left
    Node* mulNodeLeft = nodeInitType(Mul, sumNodeRight);
    sumNodeRight->left = mulNodeLeft;
    // main -> right -> left -> right
    mulNodeLeft->right = diff_(nodeToDiff->left, mulNodeLeft);
    // main -> right -> left -> left
    Node* divNodeLeft = nodeInitType(Div, mulNodeLeft);
    mulNodeLeft->left = divNodeLeft;
    divNodeLeft->left = nodeCopyPrev(nodeToDiff->right, divNodeLeft);
    divNodeLeft->right = nodeCopyPrev(nodeToDiff->left, divNodeLeft);
    // main -> right -> right
    Node* mulNodeRight = nodeInitType(Mul, sumNodeRight);
    sumNodeRight->right = mulNodeRight;
    // main -> right -> right -> left
    mulNodeRight->left = diff_(nodeToDiff->right, mulNodeRight);
    // main -> right -> right -> right
    Node* lnNodeRight = nodeInitType(Ln, mulNodeRight);
    mulNodeRight->right = lnNodeRight;
    lnNodeRight->left = nodeCopyPrev(nodeToDiff->left, lnNodeRight);

    return mainMulNode;
}

Node* diff_sin(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Sin);

    // main
    Node* mainMulNode = nodeInitType(Mul, prevNode);
    // main -> left
    Node* cosNodeLeft = nodeInitType(Cos, mainMulNode);
    mainMulNode->left = cosNodeLeft;
    // main -> left -> left (un)
    cosNodeLeft->left = nodeCopyPrev(nodeToDiff->left, cosNodeLeft);
    // main -> right
    mainMulNode->right = diff_(nodeToDiff->left, mainMulNode);

    return mainMulNode;
}

Node* diff_cos(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Cos);

    // main
    Node* mainUnaryMinusNode = nodeInitType(UnaryMinus, prevNode);
    // main -> left (un)
    Node* mulNode = nodeInitType(Mul, mainUnaryMinusNode);
    mainUnaryMinusNode->left = mulNode;
    // main -> left -> left
    Node* sinNodeLeft = nodeInitType(Sin, mulNode);
    mulNode->left = sinNodeLeft;
    // main -> left -> left -> left (un)
    sinNodeLeft->left = nodeCopyPrev(nodeToDiff->left, sinNodeLeft);
    // main -> left -> right
    mulNode->right = diff_(nodeToDiff->left, mulNode);

    return mainUnaryMinusNode;
}

Node* diff_ln(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Ln);

    // main
    Node* mainDivNode = nodeInitType(Div, prevNode);
    // main -> left
    mainDivNode->left = diff_(nodeToDiff->left, mainDivNode);
    // main -> right
    mainDivNode->right = nodeCopyPrev(nodeToDiff->left, mainDivNode);

    return mainDivNode;
}

Node* diff_var(Node* nodeToDiff, Node* prevNode){ //FIXME
    assert(nodeToDiff->type == Var);

    if(strCompareIntInt(nodeToDiff->ptrValue, diffVar))
        return initNumNode(1, prevNode);
    else
        return initNumNode(0, prevNode);
}

Node* diff_num(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Num);

    return initNumNode(0, prevNode);
}

#pragma clang diagnostic pop