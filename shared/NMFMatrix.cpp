//NMFMatrix.cpp
//Matthew Dyer
//Created on 6/1/2017
//Last Modified: 6/1/2017

#include "NMFMatrix.h"

NMFMatrix::NMFMatrix(){
}

NMFMatrix::NMFMatrix(int rowss, int cols, double (*functionPtr)(int, int)){
	rows = rowss;
	columns = cols;
	errorFunction = functionPtr;
	matrix = MatrixXd::Zero(rows,columns);
	functions = new ProbFunc**[rows];
	for(int i =0; i < rows; ++i){
		functions[i] = new ProbFunc*[columns];
		for(int j =0; j < columns; ++j){
			functions[i][j] = new HistoPF();
		}
	}	

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

