//NMFMatrix.cpp
//Matthew Dyer
//Created on 6/1/2017
//Last Modified: 6/1/2017

#include "NMFMatrix.h"

NMFMatrix::NMFMatrix(){
}

NMFMatrix::~NMFMatrix(){
	for(int i =0; i < rows; ++i){
		for(int j =0; j < columns; ++j){
			delete functions[i][j];
		}
	}

	for(int i = 0; i < rows; ++i){
		delete functions[i];
	}

	delete functions;
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

NMFMatrix::NMFMatrix(int rowss, int cols){
	rows = rowss;
	columns = cols;
	matrix = MatrixXd::Zero(rows,columns);
	functions = new ProbFunc**[rows];
	for(int i =0; i < rows; ++i){
		functions[i] = new ProbFunc*[columns];
		for(int j =0; j < columns; ++j){
			functions[i][j] = new HistoPF();
		}
	}	
}

//Size in terms of doubles
int NMFMatrix::size(){
	int rv = 0;
	for(int y =0; y < matrix.rows(); ++y){
		for(int x = 0; x < matrix.cols(); ++x){
			rv += functions[y][x]->dataSize();		
		}
	}
	return rv;
}

void NMFMatrix::read(double* data){

	Map<MatrixXd> mapper(data,matrix.rows(),matrix.cols());
	matrix = mapper;

	for(int y = 0; y < matrix.rows(); ++y){
		for(int x = 0; x < matrix.rows(); ++x){
			functions[y][x]->fromDoubles(data);
			data += functions[y][x]->dataSize();
		}
	}
}

void NMFMatrix::write(double* data){
	memcpy(data,matrix.data(),(matrix.size()*sizeof(double)));
	for(int y = 0; y < matrix.rows(); ++y){
		for(int x = 0; x < matrix.rows(); ++x){
			functions[y][x]->toDoubles(data);
			data += functions[y][x]->dataSize();
		}
	}
}


void NMFMatrix::write(string filename){
	ofstream fout;
	fout.open(filename);
	for(int y = 0; y < matrix.rows(); ++y){
		for(int x = 0; x < matrix.cols(); ++x){
			fout << matrix(y,x);
			if(x != matrix.cols()-1)
				fout << ",";
		}
		fout << "\n";
	}
	fout.close();
}

