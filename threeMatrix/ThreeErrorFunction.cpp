#include "ThreeErrorFunction.h"

ThreeErrorFunction::ThreeErrorFunction(ThreeState* st):ErrorFunction(st){
	state = st;
}

double ThreeErrorFunction::error(){
	newExpression.noalias() = state->expression;
    newExpression.noalias() -= ((state->coefficients.matrix*(state->weights.matrix*state->patterns.matrix)));
    return (newExpression.cwiseAbs()).sum();		
}
