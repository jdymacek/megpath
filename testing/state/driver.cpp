#include "State.h"

int main(int argc, char** argv){
	State* state = new State();
	string filename = string(argv[1]);
		
	state->load(filename);

	state->coefficients.resize(5,state->coefficients.columns);
	MatrixXd expression = state->coefficients.matrix * state->patterns.matrix;

	cout << expression << endl;
	
	return 0;
}