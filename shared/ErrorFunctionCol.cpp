#include "ErrorFunctionCol.h"

ErrorFunctionCol::ErrorFunctionCol(State* st):ErrorFunction(st){
	patterns = &state->patterns;
	coefficients = &state->coefficients;
}

double ErrorFunctionCol::fastError(int y,int x){
    //multiply row from cofficients to pattern matrix
    newExpression.noalias() = state->expression.col(x);
    newExpression.noalias() -= (coefficients->matrix * patterns->matrix.col(x));
    return (newExpression.cwiseAbs()).sum();
}
