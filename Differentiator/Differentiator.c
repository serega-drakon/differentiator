#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "Differentiator.h"
#include "../InputOutput/InputOutput.h"

#define REQUIRE(symbol) (line[*ptrPos] == symbol)
#define ISDIGIT (isdigit(line[*ptrPos]))
#define ISCHAR ((line[*ptrPos] >= 'a' && line[*ptrPos] <= 'z') || (line[*ptrPos] >= 'A' && line[*ptrPos] <= 'Z') || line[*ptrPos] == '_')

Node* expr_sum_sub(const int line[], unsigned *ptrPos);
Node* expr_sum_sub_(const int line[], unsigned *ptrPos, Node *firstArg);
Node* expr_unaryPlusMinus(const int line[], unsigned *ptrPos);
Node* expr_mul(const int line[], unsigned *ptrPos);
Node* expr_mul_(const int line[], unsigned *ptrPos, Node* firstArg);
Node* expr_div(const int line[], unsigned *ptrPos);
Node* expr_div_(const int line[], unsigned *ptrPos, Node* firstArg);
Node* expr_pwr(const int line[], unsigned *ptrPos);
Node* expr_pwr_(const int line[], unsigned *ptrPos, Node* firstArg);
Node* expr_sin_cos(const int line[], unsigned *ptrPos);
Node* expr_other(const int line[], unsigned *ptrPos);

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

Node* expr(const int inputLine[], unsigned sizeOfline){

    int* line = malloc(sizeof(int) * sizeOfline);
    unsigned pos = 0;
    Node* result;

    if(!exprLineCheck(line, inputLine)) {
        result = expr_sum_sub(line, &pos);
        free(line);
    }
    else {
        printf("syntax error: there are unclosed braces.\n");
        result = nodeInit();
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
        Node* sumNode = nodeInit();
        sumNode->type = Sum;
        sumNode->left = firstArg;
        firstArg->prev = sumNode;
        Node* secondArg = expr_mul(line, ptrPos);
        sumNode->right = secondArg;
        secondArg->prev = sumNode;
        return expr_sum_sub_(line, ptrPos, sumNode);
    }
    else if(REQUIRE('-')){
        (*ptrPos)++;
        Node* subNode = nodeInit();
        subNode->type = Sub;
        subNode->left = firstArg;
        firstArg->prev = subNode;
        Node* secondArg = expr_mul(line, ptrPos);
        subNode->right = secondArg;
        secondArg->prev = subNode;
        return expr_sum_sub_(line, ptrPos, subNode);
    }
    else
        return firstArg;
}

Node* expr_unaryPlusMinus(const int line[], unsigned *ptrPos){

    if(REQUIRE('+')){
        (*ptrPos)++;
        Node* unaryPlusNode = nodeInit();
        unaryPlusNode->type = UnaryPlus;
        Node* argument = expr_mul(line, ptrPos);
        unaryPlusNode->left = argument;
        argument->prev = unaryPlusNode;
        return unaryPlusNode;
    }
    else if(REQUIRE('-')){
        (*ptrPos)++;
        Node* unaryMinusNode = nodeInit();
        unaryMinusNode->type = UnaryMinus;
        Node* argument = expr_mul(line, ptrPos);
        unaryMinusNode->left = argument;
        argument->prev = unaryMinusNode;
        return unaryMinusNode;
    }
    else
        return expr_mul(line, ptrPos);
}

Node* expr_mul(const int line[], unsigned *ptrPos){

    Node* firstArg = expr_div(line, ptrPos);
    return expr_mul_(line, ptrPos, firstArg);
}

Node* expr_mul_(const int line[], unsigned *ptrPos, Node* firstArg){
    assert(firstArg != NULL);

    if(REQUIRE('*')){
        (*ptrPos)++;
        Node* mulNode = nodeInit();
        mulNode->type = Mul;
        mulNode->left = firstArg;
        firstArg->prev = mulNode;
        Node* secondArg = expr_div(line, ptrPos);
        mulNode->right = secondArg;
        secondArg->prev = mulNode;
        return expr_mul_(line, ptrPos, mulNode);
    }
    else
        return firstArg;
}

Node* expr_div(const int line[], unsigned *ptrPos){

    Node* firstArg = expr_pwr(line, ptrPos);
    return expr_div_(line, ptrPos, firstArg);
}

Node* expr_div_(const int line[], unsigned *ptrPos, Node* firstArg){\
    assert(firstArg != NULL);

    if(REQUIRE('/')){
        (*ptrPos)++;
        Node* divNode = nodeInit();
        divNode->type = Div;
        divNode->left = firstArg;
        firstArg->prev = divNode;
        Node* secondArg = expr_pwr(line, ptrPos);
        divNode->right = secondArg;
        secondArg->prev = divNode;
        return expr_div_(line, ptrPos, divNode);
    }
    else
        return firstArg;
}

Node* expr_pwr(const int line[], unsigned *ptrPos){

    Node* firstArg = expr_sin_cos(line, ptrPos);
    return expr_pwr_(line, ptrPos, firstArg);
}

Node* expr_pwr_(const int line[], unsigned *ptrPos, Node* firstArg){
    assert(firstArg != NULL);

    if(REQUIRE('^')){
        (*ptrPos)++;
        Node* pwrNode = nodeInit();
        pwrNode->type = Pwr;
        pwrNode->left = firstArg;
        firstArg->prev = pwrNode;
        Node* secondArg = expr_sin_cos(line, ptrPos);
        pwrNode->right = secondArg;
        secondArg->prev = pwrNode;
        return expr_pwr_(line, ptrPos, pwrNode);
    }
    else
        return firstArg;
}

Node* expr_sin_cos(const int line[], unsigned *ptrPos){

    if(strCompareIntChar(&line[*ptrPos], "sin(")){
        (*ptrPos) += 3;
        Node* sinNode = nodeInit();
        sinNode->type = Sin;
        Node* argument = expr_other(line, ptrPos);
        sinNode->left = argument;
        argument->prev = sinNode;
        return sinNode;
    }
    else if(strCompareIntChar(&line[*ptrPos], "cos(")){
        (*ptrPos) += 3;
        Node* cosNode = nodeInit();
        cosNode->type = Cos;
        Node* argument = expr_other(line, ptrPos);
        cosNode->left = argument;
        argument->prev = cosNode;
        return cosNode;
    }
    else
        return expr_other(line, ptrPos);
}

#define MAXVAR 100
#define MAXOP 100

Node* expr_other(const int line[], unsigned *ptrPos) {

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
    else if(ISDIGIT){ //пока трахаюсь только с целыми //FIXME добавить проверку после

        Node* numNode = nodeInit();
        numNode->type = Num;

        unsigned value = 0;
        do
            value = value * 10 + line[(*ptrPos)++] - '0';
        while(ISDIGIT);

        numNode->value = value;
        return numNode;
    }
    else if(ISCHAR){ //var

        Node* varNode = nodeInit();
        varNode->ptrValue = malloc(sizeof(int) * MAXVAR);
        varNode->type = Var;

        unsigned i = 0;

        do
            ((int*)varNode->ptrValue)[i++] = line[(*ptrPos)++];
        while((ISCHAR || ISDIGIT) && (i < MAXVAR - 1));
        ((int*)varNode->ptrValue)[i] = '\0';

        return varNode;
    }

    printf("error\n");
    Node* errorNode = nodeInit();
    errorNode->type = Error;
    return errorNode;
}
#pragma clang diagnostic pop