//NMFMatrix.cpp
//Matthew Dyer
//Created on 6/1/2017
//Last Modified: 6/1/2017

#include "NMFMatrix.h"

NMFMatrix::NMFMatrix(){
}

void NMFMatrix::writeNMFMatrix(NMFMatrix& mat,string filename){
	ofstream fout;
	fout.open(filename);
	for(int y = 0; y < mat.matrix.rows(); ++y){
		for(int x = 0; x < mat.matrix.cols(); ++x){
			fout << mat.matrix(y,x);
			if(x != mat.matrix.cols()-1)
				fout << ",";
		}
		fout << "\n";
	}
	fout.close();
}


void NMFMatrix::createNMFMatrix(NMFMatrix& rv,int rows,int columns,double (*functionPtr)(int, int)){
	rv.errorFunction = functionPtr;
	rv.matrix = MatrixXd::Zero(rows,columns);
	rv.rows = rows;
	rv.columns = columns;
	rv.functions = new ProbFunc**[rows];
	for(int i =0; i < rows; ++i){
		rv.functions[i] = new ProbFunc*[columns];
		for(int j =0; j < columns; ++j){
			rv.functions[i][j] = new HistoPF();
		}
	}	
}
