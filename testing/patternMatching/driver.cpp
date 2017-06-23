#include "State.h"

void normalizeMatrix(MatrixXd& mat){
	double min = mat.minCoeff();
	mat = mat.array() - min;
	double max = mat.maxCoeff();
	mat = mat/max;
}


int main(int argc, char** argv){
   /* State* state = new State();
    string filename = string(argv[1]);

    state->load(filename);

    state->coefficients.resize(5,state->coefficients.columns);
    MatrixXd expression = state->coefficients.matrix * state->patterns.matrix;

    cout << expression << endl;
*/

	MatrixXd pattern(3,5);
	pattern << 0.1,0.35,0.5,0.65,0.8,0.7,0.5,0.3,0.1,0.0,0.1,0.25,0.5,0.25,0.1; 
	cout << "pattern matrix:" << endl;
	cout << pattern << endl;
	MatrixXd other(3,5);
	other <<	0,0.15,0.4,0.15,0,
				0.6,0.4,0.2,0.1,0,
				0.1,0.15,0.2,0.25,0.3;
	other = other*0.5;

	cout << "other matrix:" << endl;
	cout << other << endl;


	PermutationMatrix<Dynamic> perm(pattern.rows());
	perm.setIdentity();

	for(int i =0; i < pattern.rows(); ++i){
		perm.indices().data()[i] = i;
	}

	int rows = pattern.rows();
	int index = 0;
	double minError = -1;
	for(int i =0; i < rows; ++i){
		MatrixXd p = pattern.row(i);
		normalizeMatrix(p);
		minError = -1;
		for(int j =index; j < rows; ++j){
			//Get Pattern
			MatrixXd q = other.row(perm.indices().data()[j]);
			normalizeMatrix(q);
	
			//Compare to p
			q = q-p;
			double e = q.cwiseAbs().sum();		
			if(e < minError || minError == -1){
				minError = e;
				cout << "index: " << index << " j:" << perm.indices().data()[j] << endl;
		        cout << "error: " << minError << endl;
				if(j != index){
					swap(perm.indices().data()[index],perm.indices().data()[j]);
				}
				cout << perm.indices() << endl;
			}
		}
		//move past the last mapping
		index += 1;
	}

	cout << "permuted pattern:" << endl;
	cout << perm*pattern << endl;	

    return 0;
}
