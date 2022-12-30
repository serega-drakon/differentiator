#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include <assert.h>
#include "diff.h"
#include "../InputOutput/InputOutput.h"

Node* diff_(Node* nodeToDiff);

const int* diffVar;

Node* diff(Node *exprNode, const int var[]) {
    if(exprNode == NULL || var == NULL) {
        printf("diff: error NULL exprNode or var");
        return NULL;
    }
    diffVar = var;
    return  diff_(exprNode);
}

Node* diff_sum(Node* nodeToDiff);
Node* diff_sub(Node* nodeToDiff);
Node* diff_unaryPlus(Node* nodeToDiff);
Node* diff_unaryMinus(Node* nodeToDiff);
Node* diff_mul(Node* nodeToDiff);
Node* diff_div(Node* nodeToDiff);
Node* diff_pwr(Node* nodeToDiff);
Node* diff_sin(Node* nodeToDiff);
Node* diff_cos(Node* nodeToDiff);
Node* diff_ln(Node* nodeToDiff);
Node* diff_var(Node* nodeToDiff);
Node* diff_num(Node* nodeToDiff);

Node* diff_(Node* nodeToDiff){      //FIXME: check for errors
    switch(nodeToDiff->type){
        case Sum:
            return diff_sum(nodeToDiff);
        case Sub:
            return diff_sub(nodeToDiff);
        case UnaryPlus:
            return diff_unaryPlus(nodeToDiff);
        case UnaryMinus:
            return diff_unaryMinus(nodeToDiff);
        case Mul:
            return diff_mul(nodeToDiff);
        case Div:
            return diff_div(nodeToDiff);
        case Pwr:
            return diff_pwr(nodeToDiff);
        case Sin:
            return diff_sin(nodeToDiff);
        case Cos:
            return diff_cos(nodeToDiff);
        case Ln:
            return diff_ln(nodeToDiff);
        case Var:
            return diff_var(nodeToDiff);
        case Num:
            return diff_num(nodeToDiff);
        default:
            return nodeToDiff;
    }
}

Node* diff_sum(Node* nodeToDiff){
    assert(nodeToDiff->type == Sum);
    // main
    Node* mainSumNode = nodeInitType(Sum);
    MEM_CHECK(mainSumNode);
    mainSumNode->left = diff_(nodeToDiff->left);
    mainSumNode->right = diff_(nodeToDiff->right);
    return mainSumNode;
}

Node* diff_sub(Node* nodeToDiff){
    assert(nodeToDiff->type == Sub);
    // main
    Node* mainSubNode = nodeInitType(Sub);
    MEM_CHECK(mainSubNode);
    mainSubNode->left = diff_(nodeToDiff->left);
    mainSubNode->right = diff_(nodeToDiff->right);
    return mainSubNode;
}

Node* diff_unaryPlus(Node* nodeToDiff){
    assert(nodeToDiff->type == UnaryPlus);
    // main
    Node* mainUnaryPlusNode = nodeInitType(UnaryPlus);
    MEM_CHECK(mainUnaryPlusNode);
    mainUnaryPlusNode->left = diff_(nodeToDiff->left);
    return mainUnaryPlusNode;
}

Node* diff_unaryMinus(Node* nodeToDiff){
    assert(nodeToDiff->type == UnaryMinus);
    // main
    Node* mainUnaryMinusNode = nodeInitType(UnaryMinus);
    MEM_CHECK(mainUnaryMinusNode);
    mainUnaryMinusNode->left = diff_(nodeToDiff->left);
    return mainUnaryMinusNode;
}

Node* diff_mul(Node* nodeToDiff){
    assert(nodeToDiff->type == Mul);
    // main
    Node* mainSumNode = nodeInitType(Sum);
    MEM_CHECK(mainSumNode);

    Node* mulNodeLeft = nodeInitType(Mul);
    MEM_CHECK(mulNodeLeft);
    mainSumNode->left = mulNodeLeft;
    mulNodeLeft->left = diff_(nodeToDiff->left);
    mulNodeLeft->right = nodeCopy(nodeToDiff->right);

    Node* mulNodeRight = nodeInitType(Mul);
    MEM_CHECK(mulNodeRight);
    mainSumNode->right = mulNodeRight;
    mulNodeRight->left = nodeCopy(nodeToDiff->left);
    mulNodeRight->right = diff_(nodeToDiff->right);

    return mainSumNode;
}

Node* diff_div(Node* nodeToDiff){
    assert(nodeToDiff->type == Div);
    // main
    Node* mainDivNode = nodeInitType(Div);
    MEM_CHECK(mainDivNode);

    Node* subNodeLeft = nodeInitType(Sub);
    MEM_CHECK(subNodeLeft);
    mainDivNode->left = subNodeLeft;

    Node* mulNodeLeft = nodeInitType(Mul);
    MEM_CHECK(mulNodeLeft);
    subNodeLeft->left = mulNodeLeft;
    mulNodeLeft->left = diff_(nodeToDiff->left);
    mulNodeLeft->right = nodeCopy(nodeToDiff->right);

    Node* mulNodeRight = nodeInitType(Mul);
    MEM_CHECK(mulNodeRight);
    subNodeLeft->right = mulNodeRight;
    mulNodeRight->left = nodeCopy(nodeToDiff->left);
    mulNodeRight->right = diff_(nodeToDiff->right);

    Node* pwrNodeRight = nodeInitType(Pwr);
    MEM_CHECK(pwrNodeRight);
    mainDivNode->right = pwrNodeRight;
    pwrNodeRight->left = nodeCopy(nodeToDiff->right);
    pwrNodeRight->right = initNumNode(2);

    return mainDivNode;
}

Node* diff_pwr(Node* nodeToDiff){
    assert(nodeToDiff->type == Pwr);
    // main
    Node* mainMulNode = nodeInitType(Mul);
    MEM_CHECK(mainMulNode);
    // main -> left
    mainMulNode->left = nodeCopy(nodeToDiff);
    // main -> right
    Node* sumNodeRight = nodeInitType(Sum);
    MEM_CHECK(sumNodeRight);
    mainMulNode->right = sumNodeRight;
    // main -> right -> left
    Node* mulNodeLeft = nodeInitType(Mul);
    MEM_CHECK(mulNodeLeft);
    sumNodeRight->left = mulNodeLeft;
    // main -> right -> left -> right
    mulNodeLeft->right = diff_(nodeToDiff->left);
    // main -> right -> left -> left
    Node* divNodeLeft = nodeInitType(Div);
    MEM_CHECK(divNodeLeft);
    mulNodeLeft->left = divNodeLeft;
    divNodeLeft->left = nodeCopy(nodeToDiff->right);
    divNodeLeft->right = nodeCopy(nodeToDiff->left);
    // main -> right -> right
    Node* mulNodeRight = nodeInitType(Mul);
    MEM_CHECK(mulNodeRight);
    sumNodeRight->right = mulNodeRight;
    // main -> right -> right -> left
    mulNodeRight->left = diff_(nodeToDiff->right);
    // main -> right -> right -> right
    Node* lnNodeRight = nodeInitType(Ln);
    MEM_CHECK(lnNodeRight);
    mulNodeRight->right = lnNodeRight;
    lnNodeRight->left = nodeCopy(nodeToDiff->left);

    return mainMulNode;
}

Node* diff_sin(Node* nodeToDiff){
    assert(nodeToDiff->type == Sin);

    // main
    Node* mainMulNode = nodeInitType(Mul);
    MEM_CHECK(mainMulNode);
    // main -> left
    Node* cosNodeLeft = nodeInitType(Cos);
    MEM_CHECK(cosNodeLeft);
    mainMulNode->left = cosNodeLeft;
    // main -> left -> left (un)
    cosNodeLeft->left = nodeCopy(nodeToDiff->left);
    // main -> right
    mainMulNode->right = diff_(nodeToDiff->left);

    return mainMulNode;
}

Node* diff_cos(Node* nodeToDiff){
    assert(nodeToDiff->type == Cos);

    // main
    Node* mainUnaryMinusNode = nodeInitType(UnaryMinus);
    MEM_CHECK(mainUnaryMinusNode);
    // main -> left (un)
    Node* mulNode = nodeInitType(Mul);
    MEM_CHECK(mulNode);
    mainUnaryMinusNode->left = mulNode;
    // main -> left -> left
    Node* sinNodeLeft = nodeInitType(Sin);
    MEM_CHECK(sinNodeLeft);
    mulNode->left = sinNodeLeft;
    // main -> left -> left -> left (un)
    sinNodeLeft->left = nodeCopy(nodeToDiff->left);
    // main -> left -> right
    mulNode->right = diff_(nodeToDiff->left);

    return mainUnaryMinusNode;
}

Node* diff_ln(Node* nodeToDiff){
    assert(nodeToDiff->type == Ln);

    // main
    Node* mainDivNode = nodeInitType(Div);
    MEM_CHECK(mainDivNode);
    // main -> left
    mainDivNode->left = diff_(nodeToDiff->left);
    // main -> right
    mainDivNode->right = nodeCopy(nodeToDiff->left);

    return mainDivNode;
}

Node* diff_var(Node* nodeToDiff){
    assert(nodeToDiff->type == Var);

    if(strCompareIntInt(nodeToDiff->ptrValue, diffVar))
        return initNumNode(1);
    else
        return initNumNode(0);
}

Node* diff_num(Node* nodeToDiff){
    assert(nodeToDiff->type == Num);

    return initNumNode(0);
}

#pragma clang diagnostic pop