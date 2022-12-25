#include <stdio.h>
#include "Node/Node.h"
#include "InputOutput/InputOutput.h"
#include "Differentiator/expr.h"

#define MAXLEN 300

int main() {
    int input [ MAXLEN ];
    Node* result = NULL;

    FILE* output = fopen("../output.txt", "w");

    if(getlineCMD(input, MAXLEN) > 0) {
        result = expr(input, MAXLEN);
        if(result != NULL)
            nodeSaveToFile(output, result);
        else
            printf("main: error NULL result\n");
    }

    nodeClear(result);
    fclose(output);
    return 0;
}
