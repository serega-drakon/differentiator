#include <stdio.h>
#include "Node/Node.h"
#include "InputOutput/InputOutput.h"
#include "Differentiator/Differentiator.h"

#define MAXLEN 300

int main() {
    int input [MAXLEN];
    Node* result;

    FILE* output = fopen("../output.txt", "w");

    if(getlineCMD(input, MAXLEN) > 0) {
        result = expr(input);
        nodeSaveToFile(output, result);
    }

    fclose(output);
    return 0;
}
