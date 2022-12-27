#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include "expr.h"
#include "../InputOutput/InputOutput.h"

#define REQUIRE(symbol) (line[*ptrPos] == symbol)

#define ISDIGIT (isdigit(line[*ptrPos]))

#define ISCHAR ((line[*ptrPos] >= 'a' && line[*ptrPos] <= 'z') \
 || (line[*ptrPos] >= 'A' && line[*ptrPos] <= 'Z') || line[*ptrPos] == '_')

#define RET_ERROR_NODE    do{   \
printf("expr: error by the sybmol \"%c\" at pos %d\n", line[*ptrPos], *ptrPos);\
Node* errorNode = nodeInit();   \
errorNode->type = Error;        \
return errorNode;} while(0)

#define MEM_ERR do{printf("memory error\n"); return 0;} while(0)

#define RET_UNARY_NODE(typeNode, nextFunc) do{\
Node* unarNode = nodeInit();\
if(unarNode == NULL) MEM_ERR;\
unarNode->type = typeNode;\
Node* argument = nextFunc(line, ptrPos);\
unarNode->left = argument;\
argument->prev = unarNode;\
return unarNode;} while(0)

#define RET_BINARY_NODE(currFunc, typeNode, nextFunc) do{ \
Node* newNode = nodeInit(); \
if(newNode == NULL) MEM_ERR;\
newNode->type = typeNode;        \
newNode->left = firstArg;   \
firstArg->prev = newNode;   \
Node* secondArg = nextFunc(line, ptrPos);\
newNode->right = secondArg; \
secondArg->prev = newNode;  \
return currFunc(line, ptrPos, newNode);\
} while(0)

Node* expr_sum_sub(const int line[], unsigned *ptrPos);
Node* expr_sum_sub_(const int line[], unsigned *ptrPos, Node *firstArg);
Node* expr_unaryPlusMinus(const int line[], unsigned *ptrPos);
Node* expr_mul_div(const int line[], unsigned *ptrPos);
Node* expr_mul_div_(const int line[], unsigned *ptrPos, Node* firstArg);
Node* expr_pwr(const int line[], unsigned *ptrPos);
Node* expr_pwr_(const int line[], unsigned *ptrPos, Node* firstArg);
Node* expr_sin_cos_ln(const int line[], unsigned *ptrPos);
Node* expr_other(const int line[], unsigned *ptrPos);

//для лучшего определения ошибок после введения переменной или цифры
//ака допустимые символы после них
int isValidAfterVal(int a){
    switch(a){
    case '+': case '-': case '*': case '/': case '^': case ')': case '\0':
        return 1;
    default:
        return 0;
    }
}

///подразумеваю, что clearedline достаточно длинная
int exprLineCheck(int cleared[], const int source[]){

    int countOfDisclosed = 0;
    int j = 0;

    for(unsigned i = 0; source[i] != '\0'; i++){
        if(source[i] == '(')
            countOfDisclosed++;
        else if(source[i] == ')')
            countOfDisclosed--;
        if(source[i] != ' ' && source[i] != '\t' && source[i] != '\n')
            cleared[j++] = source[i];
    }
    cleared[j] = '\0';

    return countOfDisclosed;
}

Node* expr(const int inputLine[], unsigned sizeOfline){     //FIXME: use nodeInitType

    int* line = malloc(sizeof(int) * sizeOfline);
    if(line == NULL) MEM_ERR;
    unsigned pos = 0;
    Node* result;

    if(!exprLineCheck(line, inputLine)) {
        result = expr_sum_sub(line, &pos);
        free(line);
    }
    else {
        printf("syntax error: there are unclosed braces.\n");
        result = nodeInit();
        if(result == NULL) MEM_ERR;
        result->type = Error;
    }
    return result;
}

Node *expr_sum_sub(const int line[], unsigned *ptrPos) {
    assert(line != NULL);

    Node* firstArg = expr_unaryPlusMinus(line, ptrPos);
    return expr_sum_sub_(line, ptrPos, firstArg);
}

Node *expr_sum_sub_(const int line[], unsigned *ptrPos, Node *firstArg) {
    assert(firstArg != NULL);

    if(REQUIRE('+')){
        (*ptrPos)++;
        RET_BINARY_NODE(expr_sum_sub_, Sum, expr_mul_div);
    }
    else if(REQUIRE('-')){
        (*ptrPos)++;
        RET_BINARY_NODE(expr_sum_sub_, Sub, expr_mul_div);
    }
    else
        return firstArg;
}

Node* expr_unaryPlusMinus(const int line[], unsigned *ptrPos){

    if(REQUIRE('+')){
        (*ptrPos)++;
        RET_UNARY_NODE(UnaryPlus, expr_mul_div);
    }
    else if(REQUIRE('-')){
        (*ptrPos)++;
        RET_UNARY_NODE(UnaryMinus, expr_mul_div);
    }
    else
        return expr_mul_div(line, ptrPos);
}

Node* expr_mul_div(const int line[], unsigned *ptrPos){

    Node* firstArg = expr_pwr(line, ptrPos);
    return expr_mul_div_(line, ptrPos, firstArg);
}

Node* expr_mul_div_(const int line[], unsigned *ptrPos, Node* firstArg){
    assert(firstArg != NULL);

    if(REQUIRE('*')){
        (*ptrPos)++;
        RET_BINARY_NODE(expr_mul_div_, Mul, expr_pwr);
    }
    else if (REQUIRE('/')) {
        (*ptrPos)++;
        RET_BINARY_NODE(expr_mul_div_, Div, expr_pwr);
    }
    else
        return firstArg;
}

Node* expr_pwr(const int line[], unsigned *ptrPos){

    Node* firstArg = expr_sin_cos_ln(line, ptrPos);
    return expr_pwr_(line, ptrPos, firstArg);
}

Node* expr_pwr_(const int line[], unsigned *ptrPos, Node* firstArg){
    assert(firstArg != NULL);

    if(REQUIRE('^')){
        (*ptrPos)++;
        RET_BINARY_NODE(expr_pwr_, Pwr, expr_sin_cos_ln);
    }
    else
        return firstArg;
}

Node* expr_sin_cos_ln(const int line[], unsigned *ptrPos){

    if(strCompareIntChar(&line[*ptrPos], "sin(")){
        (*ptrPos) += 3;
        RET_UNARY_NODE(Sin, expr_other);
    }
    else if(strCompareIntChar(&line[*ptrPos], "cos(")){
        (*ptrPos) += 3;
        RET_UNARY_NODE(Cos, expr_other);
    }
    else if (strCompareIntChar(&line[*ptrPos], "ln(")){
        (*ptrPos) += 2;
        RET_UNARY_NODE(Ln, expr_other);
    } else
        return expr_other(line, ptrPos);
}

#define MAXOP 100

Node* expr_other(const int line[], unsigned *ptrPos) {  //FIXME: simplify

    if(REQUIRE('(')){
        (*ptrPos)++;
        Node* result = expr_sum_sub(line, ptrPos);

        if(REQUIRE(')')){
            (*ptrPos)++;
            return result;
        }
        else{
            printf("error no brace, pos: %d\n", *ptrPos);
        }
    }
    else if(ISDIGIT){

        Node* numNode = nodeInit();
        if(numNode == NULL) MEM_ERR;
        numNode->ptrValue = malloc(sizeof(double));
        if(numNode->ptrValue == NULL) MEM_ERR;
        numNode->type = Num;

        unsigned i = 0;
        unsigned point = 0;
        char op[ MAXOP ];

        do{
            if(REQUIRE('.'))
                point++;
            op[i++] = (char) line[(*ptrPos)++];
        } while((ISDIGIT || REQUIRE('.')) && point < 2 && i < MAXOP - 1
        && line[*ptrPos] >= CHAR_MIN && line[*ptrPos] <= CHAR_MAX);

        if(point < 2 && isValidAfterVal(line[*ptrPos])){
            op[i] = '\0';
            *(double*)numNode->ptrValue = strtod(op, NULL);
            return numNode;
        }
        else
            RET_ERROR_NODE;
    }
    else if(ISCHAR){ //var

        Node* varNode = nodeInit();
        if(varNode == NULL) MEM_ERR;
        varNode->ptrValue = malloc(sizeof(int) * MAXVAR);
        if(varNode->ptrValue == NULL) MEM_ERR;
        varNode->type = Var;

        unsigned i = 0;

        do
            ((int*)varNode->ptrValue)[i++] = line[(*ptrPos)++];
        while((ISCHAR || ISDIGIT) && (i < MAXVAR - 1));
        ((int*)varNode->ptrValue)[i] = '\0';

        if(isValidAfterVal(line[*ptrPos]))
            return varNode;
        else
            RET_ERROR_NODE;
    }

    printf("error\n");
    RET_ERROR_NODE;
}
#pragma clang diagnostic pop