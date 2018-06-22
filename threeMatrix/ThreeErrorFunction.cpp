#include "ThreeErrorFunction.h"

ThreeErrorFunction::ThreeErrorFunction(ThreeState* st):ErrorFunction(st){
	state = st;
}

double ThreeErrorFunction::error(){
	cout << "Rows C:" << state->coefficients.rows << " W:" << state->weights.rows << " P:" << state->patterns.rows << endl;
	cout << "Columns C:" << state->coefficients.columns << " W:" << state->weights.columns << " P:" << state->patterns.columns << endl;
	newExpression.noalias() = state->expression;
   	MatrixXd toMult = (state->weights.matrix*state->patterns.matrix);
	newExpression.noalias() -= ((state->coefficients.matrix*toMult));
    return (newExpression.cwiseAbs()).sum();		
}
