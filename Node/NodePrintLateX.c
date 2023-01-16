#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include <assert.h>
#include "NodePrintLateX.h"
#include "Node.h"

void recPrint(FILE* file, Node* node);

int nodePrintLateX(FILE* outputFile, Node* node){

    if(outputFile == NULL){
        printf("nodePrintLateX: error outputFile is NULL\n");
        return 1;
    }
    if(node == NULL){
        printf("nodePrintLatex: error node is NULL\n");
        return 2;
    }

    recPrint(outputFile, node);
    return 0;
}

void recSum(FILE* file, Node* node);
void recSub(FILE* file, Node* node);
void recUPlus(FILE* file, Node* node);
void recUMinus(FILE* file, Node* node);
void recMul(FILE* file, Node* node);
void recDiv(FILE* file, Node* node);
void recPwr(FILE* file, Node* node);
void recSin(FILE* file, Node* node);
void recCos(FILE* file, Node* node);
void recLn(FILE* file, Node* node);
void recVar(FILE* file, Node* node);
void recNum(FILE* file, Node* node);

void recPrint(FILE* file, Node* node){

    switch(node->type){
        case Sum:
            recSum(file, node);
            return;
        case Sub:
            recSub(file, node);
            return;
        case UPlus:
            recUPlus(file, node);
            return;
        case UMinus:
            recUMinus(file, node);
            return;
        case Mul:
            recMul(file, node);
            return;
        case Div:
            recDiv(file, node);
            return;
        case Pwr:
            recPwr(file, node);
            return;
        case Sin:
            recSin(file, node);
            return;
        case Cos:
            recCos(file, node);
            return;
        case Ln:
            recLn(file, node);
            return;
        case Var:
            recVar(file, node);
            return;
        case Num:
            recNum(file, node);
            return;
        case Error:case Empty:
            return;
        default:
            assert(0);
    }

}

void recBraces(FILE* file, Node* node){
    fprintf(file, "(");
    recPrint(file, node);
    fprintf(file, ")");
}

void recSum(FILE* file, Node* node){

    recPrint(file, node->left);

    fprintf(file, " + ");

    switch(node->right->type){

        case Sum: case Sub: case UPlus: case UMinus:
            recBraces(file, node->right);
            break;
        default:
            recPrint(file, node->right);
            break;
    }
}

void recSub(FILE* file, Node* node){

    recPrint(file, node->left);

    fprintf(file, " - ");

    switch(node->right->type){

        case Sum: case Sub: case UPlus: case UMinus:
            recBraces(file, node->right);
            break;
        default:
            recPrint(file, node->right);
            break;
    }
}

void recUPlus(FILE* file, Node* node){

    fprintf(file, "+ ");
    recPrint(file, node->left);
}

void recUMinus(FILE* file, Node* node){

    fprintf(file, "- ");
    recPrint(file, node->left);
}

void recMul(FILE* file, Node* node){

    switch(node->left->type){

        case Sum: case Sub: case UPlus: case UMinus:
            recBraces(file, node->left);
            break;
        default:
            recPrint(file, node->left);
            break;
    }

    fprintf(file, " * ");

    switch(node->right->type){

        case Sum: case Sub: case UPlus: case UMinus:
        case Mul:
            recBraces(file, node->right);
            break;
        default:
            recPrint(file, node->right);
            break;
    }
}

void recDiv(FILE* file, Node* node){

    fprintf(file, "\\frac{");

    recPrint(file, node->left);

    fprintf(file, "}{");

    recPrint(file, node->right);

    fprintf(file, "}");
}

void recPwr(FILE* file, Node* node){

    switch(node->left->type){

        case Sum: case Sub: case UPlus: case UMinus:
        case Mul: case Div:
            recBraces(file, node->left);
            break;
        default:
            recPrint(file, node->left);
            break;
    }

    fprintf(file, " ^ ");

    fprintf(file, "{ ");
    recPrint(file, node->right);
    fprintf(file, " }");
}

void recSin(FILE* file, Node* node){

    fprintf(file, "\\sin{");
    recBraces(file, node->left);
    fprintf(file, "}");
}

void recCos(FILE* file, Node* node){

    fprintf(file, "\\cos{");
    recBraces(file, node->left);
    fprintf(file, "}");
}

void recLn(FILE* file, Node* node){

    fprintf(file, "\\ln{");
    recBraces(file, node->left);
    fprintf(file, "}");
}

void recVar(FILE* file, Node* node){
    assert(node->ptrValue != NULL);

    int c;
    for(int i = 0; (c = nGetVar(node, i)) != '\0'; i++)
        putc(c, file);
}

void recNum(FILE* file, Node* node){

    fprintf(file, "%g", nGetNum(node));
}
#pragma clang diagnostic pop