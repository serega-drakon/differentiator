#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include <assert.h>
#include "diff.h"
#include "../InputOutput/InputOutput.h"

Node* diff_(Node* nodeToDiff, Node* prevNode);

const int* diffVar;

Node* diff(Node *exprNode, const int var[]) {
    if(exprNode == NULL || var == NULL) {
        printf("diff: error NULL exprNode or var");
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
    Node* mainSumNode = nodeInitTypePrev(Sum, prevNode);
    MEM_CHECK(mainSumNode);
    mainSumNode->left = diff_(nodeToDiff->left, mainSumNode);
    mainSumNode->right = diff_(nodeToDiff->right, mainSumNode);
    return mainSumNode;
}

Node* diff_sub(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Sub);
    // main
    Node* mainSubNode = nodeInitTypePrev(Sub, prevNode);
    MEM_CHECK(mainSubNode);
    mainSubNode->left = diff_(nodeToDiff->left, mainSubNode);
    mainSubNode->right = diff_(nodeToDiff->right, mainSubNode);
    return mainSubNode;
}

Node* diff_unaryPlus(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == UnaryPlus);
    // main
    Node* mainUnaryPlusNode = nodeInitTypePrev(UnaryPlus, prevNode);
    MEM_CHECK(mainUnaryPlusNode);
    mainUnaryPlusNode->left = diff_(nodeToDiff->left, mainUnaryPlusNode);
    return mainUnaryPlusNode;
}

Node* diff_unaryMinus(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == UnaryMinus);
    // main
    Node* mainUnaryMinusNode = nodeInitTypePrev(UnaryMinus, prevNode);
    MEM_CHECK(mainUnaryMinusNode);
    mainUnaryMinusNode->left = diff_(nodeToDiff->left, mainUnaryMinusNode);
    return mainUnaryMinusNode;
}

Node* diff_mul(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Mul);
    // main
    Node* mainSumNode = nodeInitTypePrev(Sum, prevNode);
    MEM_CHECK(mainSumNode);

    Node* mulNodeLeft = nodeInitTypePrev(Mul, mainSumNode);
    MEM_CHECK(mulNodeLeft);
    mainSumNode->left = mulNodeLeft;
    mulNodeLeft->left = diff_(nodeToDiff->left, mulNodeLeft);
    mulNodeLeft->right = nodeCopyPrev(nodeToDiff->right, mulNodeLeft);

    Node* mulNodeRight = nodeInitTypePrev(Mul, mainSumNode);
    MEM_CHECK(mulNodeRight);
    mainSumNode->right = mulNodeRight;
    mulNodeRight->left = nodeCopyPrev(nodeToDiff->left, mulNodeRight);
    mulNodeRight->right = diff_(nodeToDiff->right, mulNodeRight);

    return mainSumNode;
}

Node* diff_div(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Div);
    // main
    Node* mainDivNode = nodeInitTypePrev(Div, prevNode);
    MEM_CHECK(mainDivNode);

    Node* subNodeLeft = nodeInitTypePrev(Sub, mainDivNode);
    MEM_CHECK(subNodeLeft);
    mainDivNode->left = subNodeLeft;

    Node* mulNodeLeft = nodeInitTypePrev(Mul, subNodeLeft);
    MEM_CHECK(mulNodeLeft);
    subNodeLeft->left = mulNodeLeft;
    mulNodeLeft->left = diff_(nodeToDiff->left, mulNodeLeft);
    mulNodeLeft->right = nodeCopyPrev(nodeToDiff->right, mulNodeLeft);

    Node* mulNodeRight = nodeInitTypePrev(Mul, subNodeLeft);
    MEM_CHECK(mulNodeRight);
    subNodeLeft->right = mulNodeRight;
    mulNodeRight->left = nodeCopyPrev(nodeToDiff->left, mulNodeRight);
    mulNodeRight->right = diff_(nodeToDiff->right, mulNodeRight);

    Node* pwrNodeRight = nodeInitTypePrev(Pwr, mainDivNode);
    MEM_CHECK(pwrNodeRight);
    mainDivNode->right = pwrNodeRight;
    pwrNodeRight->left = nodeCopyPrev(nodeToDiff->right, pwrNodeRight);
    pwrNodeRight->right = initNumNode(2, pwrNodeRight);

    return mainDivNode;
}

Node* diff_pwr(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Pwr);
    // main
    Node* mainMulNode = nodeInitTypePrev(Mul, prevNode);
    MEM_CHECK(mainMulNode);
    // main -> left
    mainMulNode->left = nodeCopyPrev(nodeToDiff, mainMulNode);
    // main -> right
    Node* sumNodeRight = nodeInitTypePrev(Sum, mainMulNode);
    MEM_CHECK(sumNodeRight);
    mainMulNode->right = sumNodeRight;
    // main -> right -> left
    Node* mulNodeLeft = nodeInitTypePrev(Mul, sumNodeRight);
    MEM_CHECK(mulNodeLeft);
    sumNodeRight->left = mulNodeLeft;
    // main -> right -> left -> right
    mulNodeLeft->right = diff_(nodeToDiff->left, mulNodeLeft);
    // main -> right -> left -> left
    Node* divNodeLeft = nodeInitTypePrev(Div, mulNodeLeft);
    MEM_CHECK(divNodeLeft);
    mulNodeLeft->left = divNodeLeft;
    divNodeLeft->left = nodeCopyPrev(nodeToDiff->right, divNodeLeft);
    divNodeLeft->right = nodeCopyPrev(nodeToDiff->left, divNodeLeft);
    // main -> right -> right
    Node* mulNodeRight = nodeInitTypePrev(Mul, sumNodeRight);
    MEM_CHECK(mulNodeRight);
    sumNodeRight->right = mulNodeRight;
    // main -> right -> right -> left
    mulNodeRight->left = diff_(nodeToDiff->right, mulNodeRight);
    // main -> right -> right -> right
    Node* lnNodeRight = nodeInitTypePrev(Ln, mulNodeRight);
    MEM_CHECK(lnNodeRight);
    mulNodeRight->right = lnNodeRight;
    lnNodeRight->left = nodeCopyPrev(nodeToDiff->left, lnNodeRight);

    return mainMulNode;
}

Node* diff_sin(Node* nodeToDiff, Node* prevNode){
    assert(nodeToDiff->type == Sin);

    // main
    Node* mainMulNode = nodeInitTypePrev(Mul, prevNode);
    MEM_CHECK(mainMulNode);
    // main -> left
    Node* cosNodeLeft = nodeInitTypePrev(Cos, mainMulNode);
    MEM_CHECK(cosNodeLeft);
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
    Node* mainUnaryMinusNode = nodeInitTypePrev(UnaryMinus, prevNode);
    MEM_CHECK(mainUnaryMinusNode);
    // main -> left (un)
    Node* mulNode = nodeInitTypePrev(Mul, mainUnaryMinusNode);
    MEM_CHECK(mulNode);
    mainUnaryMinusNode->left = mulNode;
    // main -> left -> left
    Node* sinNodeLeft = nodeInitTypePrev(Sin, mulNode);
    MEM_CHECK(sinNodeLeft);
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
    Node* mainDivNode = nodeInitTypePrev(Div, prevNode);
    MEM_CHECK(mainDivNode);
    // main -> left
    mainDivNode->left = diff_(nodeToDiff->left, mainDivNode);
    // main -> right
    mainDivNode->right = nodeCopyPrev(nodeToDiff->left, mainDivNode);

    return mainDivNode;
}

Node* diff_var(Node* nodeToDiff, Node* prevNode){
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