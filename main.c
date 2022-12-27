#include <stdio.h>
#include "Node/Node.h"
#include "InputOutput/InputOutput.h"
#include "Differentiator/expr.h"
#include "Differentiator/diff.h"
#include "Differentiator/opt.h"

#define MAXLEN 300

int main() {

    int input [ MAXLEN ];
    Node* exprRes = NULL;
    Node* diffRes = NULL;
    FILE* outputExpr = fopen("../Debug/outputExpr.txt", "w");
    FILE* outputDiff = fopen("../Debug/outputDiff.txt", "w");
    FILE* outputOpt = fopen("../Debug/outputOpt.txt", "w");

    printf("Введите выражение:\n");

    if(getlineCMD(input, MAXLEN) > 0){
        exprRes = expr(input, MAXLEN);
        if(exprRes != NULL)
            nodeSaveToFile(outputExpr, exprRes);
        else
            printf("main: error NULL exprRes\n");

        int var[ MAXVAR ];
        printf("Введите переменную, по которой будем дифференцировать:\n");
        getlineCMD(var, MAXVAR);

        diffRes = diff(exprRes, var);
        if(diffRes != NULL)
            nodeSaveToFile(outputDiff, diffRes);
        else
            printf("main: error NULL diffRes\n");

    }

    nodeClear(exprRes);
    nodeClear(diffRes);
    fclose(outputExpr);
    fclose(outputDiff);
    fclose(outputOpt);
    return 0;
}
