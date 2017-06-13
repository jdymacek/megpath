//test file for NMFMatrix
//Matthew Dyer
//Created on 6/13/2017
//Last Modified: 6/13/2017

#include "NMFMatrix.h"
#include "ProbFunc.h"

int main(int argc, char** argv){
	NMFMatrix a = NMFMatrix(3,3);

	MatrixXd temp(3,3);
	temp << 0.1 , 0.2 , 0.3 ,
			  0.4 , 0.5 , 0.6 ,
			  0.7 , 0.8 , 0.9 ;

	a.matrix = temp;

	cout << "This is the matrix : \n" << a.matrix << endl;

	
	int bufferSize = a.size()+1;
	double* buffer = new double[bufferSize];

	cout << "Before writing buffer[1] is " << buffer[1] << endl;
	a.write(&buffer[1]);
	cout << "After writing buffer[1] is " << buffer[1] << endl;

	delete buffer;

	return 0;
}
