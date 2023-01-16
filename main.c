#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Node/Node.h"
#include "Node/NodePrintLateX.h"
#include "InputOutput/InputOutput.h"
#include "Differentiator/expr.h"
#include "Differentiator/diff.h"
#include "Differentiator/opt.h"

#define MAXLEN 300

typedef struct mainStruct_ {

    Node* exprRes;
    Node* optExprRes;
    Node* diffRes;
    Node* optDiffRes;
    FILE* outputExpr;
    FILE* outputOptExpr;
    FILE* outputDiff;
    FILE* outputOptDiff;
    FILE* outputLatex;
} mainStruct;

mainStruct* mainStructInit(){

    mainStruct* mainStruct1 = malloc(sizeof(mainStruct));
    if(mainStruct1 == NULL)
        return NULL;
    mainStruct1->exprRes = NULL;
    mainStruct1->optExprRes = NULL;
    mainStruct1->diffRes = NULL;
    mainStruct1->optDiffRes = NULL;
    mainStruct1->outputExpr = fopen("../Debug/outputExpr.txt", "w");
    mainStruct1->outputOptExpr = fopen("../Debug/outputOptExpr.txt", "w");
    mainStruct1->outputDiff = fopen("../Debug/outputDiff.txt", "w");
    mainStruct1->outputOptDiff = fopen("../Debug/outputOptDiff.txt", "w");
    mainStruct1->outputLatex = fopen("../outputLatex.txt", "w");
    return mainStruct1;
}

void mainStructFree(mainStruct* mainStruct1){

    nodeClear(mainStruct1->exprRes);
    nodeClear(mainStruct1->optExprRes);
    nodeClear(mainStruct1->diffRes);
    nodeClear(mainStruct1->optDiffRes);
    fclose(mainStruct1->outputExpr);
    fclose(mainStruct1->outputOptExpr);
    fclose(mainStruct1->outputDiff);
    fclose(mainStruct1->outputOptDiff);
    fclose(mainStruct1->outputLatex);
    free(mainStruct1);
}

void mainDebugOutput(mainStruct* mainStruct1){

    if(mainStruct1->exprRes != NULL)
        nodeSaveToFile(mainStruct1->outputExpr, mainStruct1->exprRes);
    else
        printf("mainDebugOutput: exprRes is NULL\n");

    if(mainStruct1->optExprRes != NULL)
        nodeSaveToFile(mainStruct1->outputOptExpr, mainStruct1->optExprRes);
    else
        printf("mainDebugOutput: optExprRes is NULL\n");

    if(mainStruct1->diffRes != NULL)
        nodeSaveToFile(mainStruct1->outputDiff, mainStruct1->diffRes);
    else
        printf("mainDebugOutput: diffRes is NULL\n");

    if(mainStruct1->optDiffRes != NULL)
        nodeSaveToFile(mainStruct1->outputOptDiff, mainStruct1->optDiffRes);
    else
        printf("mainDebugOutput: optDiffRes is NULL\n");

}

void pasteFromFile(FILE* from, FILE* to){
    assert(from != NULL && to != NULL);

    int c;
    while((c = getc(from)) != EOF)
        putc(c, to);
}

void mainLatex(mainStruct* mainStruct1){ //FIXME??

    FILE* lateXFile = mainStruct1->outputLatex;
    if(lateXFile == NULL){
        printf("mainLatex: error outputLatex file is NULL\n");
        return;
    }

    FILE* textFile = fopen("../text.txt", "r");
    if(textFile == NULL){
        printf("mainLatex: error textFile is NULL\n");
        return;
    }

    pasteFromFile(textFile, lateXFile);

    fprintf(lateXFile, "\\[");
    nodePrintLateX(lateXFile, mainStruct1->exprRes);
    fprintf(lateXFile, "\\]\n");

    fprintf(lateXFile, "\\end{document}");

    fclose(textFile);
}

void mainDiff(mainStruct* mainStruct1, int var[]){


}

int main() { //FIXME

    mainStruct* mainStruct1 = mainStructInit();

    int input [ MAXLEN ];

    printf("Введите выражение:\n");

    if(getlineCMD(input, MAXLEN) > 0){

        int var[ MAXVAR ];
        printf("Введите переменную, по которой будем дифференцировать:\n");
        getlineCMD(var, MAXVAR);

        mainStruct1->exprRes = expr(input, MAXLEN);
        mainStruct1->optExprRes = opt(mainStruct1->exprRes);
        mainStruct1->diffRes = diff(mainStruct1->optExprRes, var);
        mainStruct1->optDiffRes = opt(mainStruct1->diffRes);
        mainDebugOutput(mainStruct1);
        mainLatex(mainStruct1);
    }

    mainStructFree(mainStruct1);

    return 0;
}
