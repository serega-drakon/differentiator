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
#define MAXCOUNT 50
typedef struct mainStruct_ {

    Node* exprRes;
    Node* optExprRes;
    FILE* outputExpr;
    FILE* outputOptExpr;
    FILE* outputLatex;
} mainStruct;

mainStruct* mainStructInit(){

    mainStruct* mainStruct1 = malloc(sizeof(mainStruct));
    if(mainStruct1 == NULL)
        return NULL;
    mainStruct1->exprRes = NULL;
    mainStruct1->optExprRes = NULL;
    mainStruct1->outputExpr = fopen("../Debug/outputExpr.txt", "w");
    mainStruct1->outputOptExpr = fopen("../Debug/outputOptExpr.txt", "w");
    mainStruct1->outputLatex = fopen("../outputLatex.txt", "w");
    return mainStruct1;
}

void mainStructFree(mainStruct* mainStruct1){

    nodeClear(mainStruct1->exprRes);
    nodeClear(mainStruct1->optExprRes);
    fclose(mainStruct1->outputExpr);
    fclose(mainStruct1->outputOptExpr);
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
}

void pasteFromFile(FILE* from, FILE* to){
    assert(from != NULL && to != NULL);

    int c;
    while((c = getc(from)) != EOF)
        putc(c, to);
}

void pasteString(FILE* to, const int str[]){
    assert(to != NULL && str != NULL);

    for(int i = 0; str[i] != '\0'; i++)
        putc(str[i], to);
}

void mainDiff(mainStruct* mainStruct1, int var[][MAXVAR]) {

    Node* exprNode = mainStruct1->optExprRes;
    if(exprNode == NULL)
        return;

    Node* diffNode;
    Node* optDiffNode;

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

    fprintf(lateXFile, "Введена формула:\n");

    fprintf(lateXFile, "\n\\[ f (");

    nodePrintLateX(lateXFile, mainStruct1->exprRes);
    fprintf(lateXFile, "\\]\n");

    fprintf(lateXFile, "Оптимизация формулы:\n");

    fprintf(lateXFile, "\n\\[");
    nodePrintLateX(lateXFile, mainStruct1->optExprRes);
    fprintf(lateXFile, "\\]\n");

    fprintf(lateXFile, "Вывод полного дифференциала:\n");
    if(var[0][0] != '\0') {
        fprintf(lateXFile, "\n\\begin{multline*}\n \\,df (");

        pasteString(lateXFile, var[0]);

        for(int i = 1; i < MAXCOUNT && var[i][0] != '\0'; i++) {
            fprintf(lateXFile, ", ");
            pasteString(lateXFile, var[i]);
        }

        fprintf(lateXFile, ") = ");

        diffNode = diff(exprNode, var[0]);
        optDiffNode = opt(diffNode);

        fprintf(lateXFile, "\\bigg(");
        nodePrintLateX(lateXFile, optDiffNode);
        fprintf(lateXFile, "\\bigg) \\,d(");
        pasteString(lateXFile, var[0]);
        fprintf(lateXFile, ")");

        nodeClear(diffNode);
        nodeClear(optDiffNode);

        for(int i = 1; i < MAXCOUNT && var[i][0] != '\0'; i++){

            diffNode = diff(exprNode, var[i]);
            optDiffNode = opt(diffNode);

            fprintf(lateXFile, "+ \\\\ \n+ \\bigg(");
            nodePrintLateX(lateXFile, optDiffNode);
            fprintf(lateXFile, "\\bigg) \\,d(");
            pasteString(lateXFile, var[i]);
            fprintf(lateXFile, ")");

            nodeClear(diffNode);
            nodeClear(optDiffNode);
        }
        fprintf(lateXFile, "\n\\end{multline*}\n");
    }
    else
        fprintf(lateXFile, "Не введены переменные.\n");
    fprintf(lateXFile, "\n\\end{document}\n");

    fclose(textFile);
}

int main() {

    mainStruct* mainStruct1 = mainStructInit();

    int input [ MAXLEN ];

    printf("Введите выражение:\n");

    if(getlineCMD(input, MAXLEN) > 0){

        int var[ MAXCOUNT ][ MAXVAR ];
        printf("Введите переменные, по которым будем дифференцировать (пустое поле для окончания ввода):\n");

        for(int i = 0; i < MAXCOUNT && getlineCMD(var[i], MAXVAR) > 0; i++)
            ;

        mainStruct1->exprRes = expr(input, MAXLEN);
        mainStruct1->optExprRes = opt(mainStruct1->exprRes);

        mainDebugOutput(mainStruct1);

        mainDiff(mainStruct1, var);
    }

    mainStructFree(mainStruct1);

    return 0;
}
