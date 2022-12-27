#ifndef DIFFERENTIATOR_INPUTOUTPUT_H
#define DIFFERENTIATOR_INPUTOUTPUT_H

#include <stdio.h>

int strCompareIntChar(const int a[], const char b[]);
int strCompareIntInt(const int a[], const int b[]);
int getlineCMD(int input[], unsigned maxLen);
void fprintIntS(FILE* output, int str[]);
void copyStr(const int fromStr[], int toStr[]);

#endif //DIFFERENTIATOR_INPUTOUTPUT_H
