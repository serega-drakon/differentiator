#include <stdio.h>
#include "Node/Node.h"
#include "InputOutput/InputOutput.h"
#include "Differentiator/Differentiator.h"

#define MAXLEN 300

int main() {
    int input [MAXLEN];
    Node* result = NULL;

    FILE* output = fopen("../output.txt", "w");

    if(getlineCMD(input, MAXLEN) > 0) {
        result = expr(input, MAXLEN);
        nodeSaveToFile(output, result);
    }

    nodeFree(result);
    fclose(output);
    return 0;
}
