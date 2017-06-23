//State tester
//Julian Dymacek
//Matthew Dyer
//Last Modified: 6/23/2017

#include "State.h"

int main(int argc, char** argv){
	State* state = new State();
	string filename = string(argv[1]);
		
	state->load(filename);
	
	state->patterns.matrix << 0.1,0.35,0.5,0.65,0.8,
									  0.7,0.5,0.3,0.1,0,
									  0.1,0.25,0.5,0.25,0.1;
	state->coefficients.resize(3,3);
	state->coefficients.matrix << 10,11,12,20,21,22,30,31,32;

	cout << "original patterns: \n" << state->patterns.matrix << endl;

	MatrixXd toMatch(3,5);
	toMatch <<  0.6,0.4,0.2,0.1,0,
					0.1,0.35,0.5,0.65,0.8,
					0,0.15,0.4,0.15,0;

	cout << "patterns to match: \n" << toMatch << endl;

	state->patternMatch(toMatch);

	cout << "after matching patterns: \n" << state->patterns.matrix << endl;

	toMatch = toMatch * 0.5;

	cout << "new patterns to match: \n" << toMatch << endl;

	state->patternMatch(toMatch);

	cout << "after matching patterns again: \n" << state->patterns.matrix << endl;
	cout << "after matching patterns coe: \n" << state->coefficients.matrix << endl;	

/*	
	state->coefficients.resize(5,state->coefficients.columns);
	MatrixXd expression = state->coefficients.matrix * state->patterns.matrix;
	cout << expression << endl;
*/	
	return 0;
}
