#include "ErrorFunctionRow.h"

ErrorFunctionRow::ErrorFunctionRow(State* st):ErrorFunction(st){
    patterns = &state->patterns;
    coefficients = &state->coefficients;
}

double ErrorFunctionRow::fastError(int y,int x){
    //multiply row from cofficients to pattern matrix
    newExpression.noalias() = state->expression.row(y);
    newExpression.noalias() -= (coefficients->matrix.row(y) * patterns->matrix);
    return (newExpression.cwiseAbs()).sum();	
}
