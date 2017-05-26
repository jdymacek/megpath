//connnmf tester
//Matthew Dyer
//Created on 5/26/2017
//Last Modified: 5/26/2017

#include <iostream>
#include "../../../Eigen/Core"

using namespace std;
using namespace Eigen;

int main(){
	MatrixXd patterns(3,5);
	patterns << 0,0.15,0.2,0.15,0, 0,0.15,0.2,0.15,0, 0,0.15,0.2,0.15,0;

	cout << patterns << endl;

	for(int i = 0; i < patterns.rows(); ++i){
		for(int j = 0; j < patterns.cols(); ++j){
			cout << patterns(i,j) << " ";
		}
		cout << "\n";
	}

	return 0;
}
