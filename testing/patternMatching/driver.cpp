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
	testing << 10,11,12,13,20,21,22,23,30,31,32,33;
	cout << testing << endl;

	PermutationMatrix<Dynamic> perm(testing.rows());
	perm.setIdentity();

	for(int i =0; i < testing.rows(); ++i){
		perm.indices().data()[i] = testing.rows()-i-1;
	}

	cout << perm.indices() << endl;
	testing = perm*testing;
	cout << testing << endl;

	for(int i =0; i < blah.rows(); ++i){
		MatrixXd p = blah.row(i);
		//normalize pattern
		double min = p.minCoeff();
		p = p.array() - min;
		double max = p.maxCoeff();
		p = p/max;

		//get row
		for(int j =0; j < testing.rows(); ++j){
			//Get Pattern
			MatrixXd q = testing.row(j);
			//Normalize Pattern
			min = q.minCoeff();
			q = q.array() - min;
			max = q.maxCoeff();
			q = q/max;
	
			//Compare to p
			q = q-p;
			double e = q.cwiseAbs().sum();		
			if(e < minError){
				minIndex = j;
				minError = e;
				//swap instead?
			}
		}

	}

    return 0;
}
