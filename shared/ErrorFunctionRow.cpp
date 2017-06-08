#include "ErrorFunctionRow.h"

ErrorFunctionRow::ErrorFunctionRow(State* st):ErrorFunction(st){

}

double ErrorFunctionRow::fastError(int y,int x){
    //multiply row from cofficients to pattern matrix
    newExpression.noalias() = state->expression.row(y);
    newExpression.noalias() -= (state->coefficients.matrix.row(y) * state->patterns.matrix);
    return (newExpression.cwiseAbs()).sum();	
}
