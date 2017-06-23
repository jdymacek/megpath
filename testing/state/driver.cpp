//State tester
//Julian Dymacek
//Matthew Dyer
//Last Modified: 6/23/2017

#include "State.h"

int main(int argc, char** argv){
	State* state = new State();
	string filename = string(argv[1]);
		
	state->load(filename);

	cout << "original patterns: \n" << state->patterns.matrix;

	MatrixXd toMatch(3,5);
	toMatch <<  0.6,0.4,0.2,0.1,0,
					0.1,0.35,0.5,0.65,0.8,
					0,0.15,0.4,0.15,0;

	cout << "patterns to match: \n" << toMatch;

	state->patternMatch(toMatch);

	cout << "after matching patterns: \n" << state->patterns.matrix;
	
/*	
	state->coefficients.resize(5,state->coefficients.columns);
	MatrixXd expression = state->coefficients.matrix * state->patterns.matrix;
	cout << expression << endl;
*/	
	return 0;
}
