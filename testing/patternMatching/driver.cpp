#include "State.h"

int main(int argc, char** argv){
   /* State* state = new State();
    string filename = string(argv[1]);

    state->load(filename);

    state->coefficients.resize(5,state->coefficients.columns);
    MatrixXd expression = state->coefficients.matrix * state->patterns.matrix;

    cout << expression << endl;
*/
	MatrixXd testing(3,4);
	testing << 0,1,2,3,0,1,2,3,0,1,2,3;
	cout << testing << endl;

	PermutationMatrix<Dynamic> perm(testing.cols());
	perm.setIdentity();

	for(int i =0; i < testing.cols(); ++i){
		perm.indices().data()[i] = testing.cols()-i-1;
	}

	cout << perm.indices() << endl;
	testing = testing*perm;
	cout << testing << endl;

    return 0;
}
