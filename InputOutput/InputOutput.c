#include "InputOutput.h"
#include <stdio.h>

int strCompareIntChar(const int a[], const char b[]){

    int i;
    for(i = 0; a[i] != '\0' && b[i] != '\0' && a[i] == (int)b[i]; i++)
        ;
    if(b[i] == '\0')
        return 1;
    else
        return 0;
}

int strCompareIntInt(const int a[], const int b[]){

    int i;
    for(i = 0; a[i] != '\0' && b[i] != '\0' && a[i] == b[i]; i++)
        ;
    if(b[i] == '\0')
        return 1;
    else
        return 0;
}

int getlineCMD(int input[], unsigned maxLen){

    int i;
    int c;
    for(i = 0; i < maxLen - 1 && (c = getchar()) != '\n' && c != EOF; i++)
        input[i] = c;
    input[i] = '\0';
    return i;
}

void fprintIntS(FILE* output, int str[]){

    for(int i = 0; str[i] != '\0'; i++)
        putc(str[i], output);
}

void copyStr(const int fromStr[], int toStr[]){

    int i;
    for(i = 0; fromStr[i] != '\0'; i++)
        toStr[i] = fromStr[i];
    toStr[i] = '\0';
}