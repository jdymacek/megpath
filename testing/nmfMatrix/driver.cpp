//test file for NMFMatrix
//Matthew Dyer
//Created on 6/13/2017
//Last Modified: 6/13/2017

#include "NMFMatrix.h"
#include "ProbFunc.h"

int main(int argc, char** argv){
	NMFMatrix a = NMFMatrix(6,4);

	MatrixXd temp(6,4);
	temp << 0.1 , 0.2 , 0.3 , 0.4,
			0.5 , 0.6 , 0.7 , 0.8,
			0.9 , 1.0 , 1.1 , 1.2,
			1.3 , 1.4 , 1.5 , 1.6,
			1.7 , 1.8 , 1.9 , 2.0,
			2.1 , 2.2 , 2.3 , 2.4;
	a.matrix = temp;

	cout << "This is the matrix : \n" << a.matrix << endl;
	
	a.createBuffers();

//	cout << "Before writing buffer[1] is " << buffer[1] << endl;
//	a.write(&buffer[0]);
//	cout << "After writing buffer[1] is " << buffer[1] << endl;

//	Step 1: Make Block
//	Step 2: Look at Data
//	Step 3: Put into another buffer

	Range r;
	r.rowStart = 0;
	r.colStart = 2;
	r.rowEnd = 2;
	r.colEnd = 3;
	
	int ret = a.write(&a.recvBuffer[0],r);

	cout << ret << endl;
	for(int y = 0; y < ret; ++y){
		a.sendBuffer[y] = a.recvBuffer[y]+1;
	}

	NMFMatrix b = NMFMatrix(6,4);
	b.createBuffers();

	b.read(&a.sendBuffer[0],r);

	cout << b.matrix << endl;
/*	
	NMFMatrix b = NMFMatrix(r.rowSize(),r.colSize());

	b.matrix = a.matrix.block(r.rowStart,r.colStart,r.rowSize(),r.colSize());

	int bBufSize = b.size();
	double* bBuffer = new double[bBufSize];

	memcpy(bBuffer,b.matrix.data(),((r.rowSize()*r.colSize())*sizeof(double)));

	for(int y = 0; y < r.rowSize()*r.colSize(); ++y){
		bBuffer[y] = 10;
	}
	
	Map<MatrixXd> mapper(bBuffer,r.rowSize(),r.colSize());
	
	a.matrix.block(r.rowStart,r.rowEnd,r.rowSize(),r.colSize()) = mapper;

	cout << "Did I win?\n" << a.matrix << endl;

	delete bBuffer;
	delete buffer;*/

	return 0;
}
