#include "ErrorFunctionCol.h"

ErrorFunctionCol::ErrorFunctionCol(State* st):ErrorFunction(st){

}

double ErrorFunctionCol::fastError(int y,int x){
    //multiply row from cofficients to pattern matrix
    newExpression.noalias() = state->expression.col(x);
    newExpression.noalias() -= (state->coefficients.matrix * state->patterns.matrix.col(x));
    return (newExpression.cwiseAbs()).sum();
}
