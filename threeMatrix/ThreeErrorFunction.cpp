#include "ThreeErrorFunction.h"

ThreeErrorFunction::ThreeErrorFunction(ThreeState* st):ErrorFunction(st){
	state = st;
}

double ThreeErrorFunction::error(){
	newExpression.noalias() = state->expression;
    newExpression.noalias() -= ((state->coefficients.matrix*state->patterns.matrix)*state->weights.matrix);
    return (newExpression.cwiseAbs()).sum();		
}
