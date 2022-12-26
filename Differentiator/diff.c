#include <assert.h>
#include "diff.h"
#include "../Node/Node.h"

Node* diff_(Node* nodeToDiff);

Node* diff(Node* exprNode){
    if(exprNode == NULL) {
        printf("diff: error NULL exprNode");
        return NULL;
    }
    return diff_(exprNode);
}


Node* diff_sum(Node* nodeToDiff);
Node* diff_sub(Node* nodeToDiff);
Node* diff_unaryPlus(Node* nodeToDiff);
Node* diff_unaryMinus(Node* nodeToDiff);
Node* diff_mul(Node* nodeToDiff);
Node* diff_div(Node* nodeToDiff);
Node* diff_pwr(Node* nodeToDiff);
Node* diff_sin(Node* nodeToDiff);
Node* diff_cos(Node* nodeToDiff);
Node* diff_ln(Node* nodeToDiff);
Node* diff_var(Node* nodeToDiff);
Node* diff_num(Node* nodeToDiff);

Node* diff_(Node* nodeToDiff){
    switch(nodeToDiff->type){
        case Sum:
            return diff_sum(nodeToDiff);
        case Sub:
            return diff_sub(nodeToDiff);
        case UnaryPlus:
            return diff_unaryPlus(nodeToDiff);
        case UnaryMinus:
            return diff_unaryMinus(nodeToDiff);
        case Mul:
            return diff_mul(nodeToDiff);
        case Div:
            return diff_div(nodeToDiff);
        case Pwr:
            return diff_pwr(nodeToDiff);
        case Sin:
            return diff_sin(nodeToDiff);
        case Cos:
            return diff_cos(nodeToDiff);
        case Ln:
            return diff_ln(nodeToDiff);
        case Var:
            return diff_var(nodeToDiff);
        case Num:
            return diff_num(nodeToDiff);
        default:
            return nodeToDiff;
    }
}

Node* diff_sum(Node* nodeToDiff){
    assert(nodeToDiff->type == Sum);
}

Node* diff_sub(Node* nodeToDiff){
    assert(nodeToDiff->type == Sub);

}

Node* diff_unaryPlus(Node* nodeToDiff){
    assert(nodeToDiff->type == UnaryPlus);

}

Node* diff_unaryMinus(Node* nodeToDiff){
    assert(nodeToDiff->type == UnaryMinus);

}

Node* diff_mul(Node* nodeToDiff){
    assert(nodeToDiff->type == Mul);

}

Node* diff_div(Node* nodeToDiff){
    assert(nodeToDiff->type == Div);

}

Node* diff_pwr(Node* nodeToDiff){
    assert(nodeToDiff->type == Pwr);

}

Node* diff_sin(Node* nodeToDiff){
    assert(nodeToDiff->type == Sin);

}

Node* diff_cos(Node* nodeToDiff){
    assert(nodeToDiff->type == Cos);

}

Node* diff_ln(Node* nodeToDiff){
    assert(nodeToDiff->type == Ln);

}

Node* diff_var(Node* nodeToDiff){
    assert(nodeToDiff->type == Var);

}

Node* diff_num(Node* nodeToDiff){
    assert(nodeToDiff->type == Num);

}
