//NMFMatrix.cpp
//Matthew Dyer
//Created on 6/1/2017
//Last Modified: 6/12/2017

#include "NMFMatrix.h"

NMFMatrix::NMFMatrix(){
}

NMFMatrix::~NMFMatrix(){
	for(int i =0; i < rows; ++i){
		for(int j =0; j < columns; ++j){
			delete functions(i,j);
		}
	}
}

NMFMatrix::NMFMatrix(int rowss, int cols){
	rows = rowss;
	columns = cols;
	matrix = MatrixXd::Zero(rows,columns);
	functions = MatrixXp(rows,columns);

	for(int i =0; i < rows; ++i){
		for(int j =0; j < columns; ++j){
			functions(i,j) = new PiecewisePF();
		}
	}	
}

//Size in terms of doubles
//this could be cached
int NMFMatrix::size(){
	int rv = 0;
	for(int y =0; y < matrix.rows(); ++y){
		for(int x = 0; x < matrix.cols(); ++x){
			rv += functions(y,x)->dataSize();		
		}
	}
	return matrix.size()+rv;
}

void NMFMatrix::reset(){
    rows = matrix.rows();
    columns = matrix.cols();
	for(int y =0; y < matrix.rows(); ++y){
        for(int x = 0; x < matrix.cols(); ++x){
            functions(y,x)->reset();
			if(functions(y,x)->size() > 1){
	            for(int k=0; k < functions(y,x)->size(); ++k){
                    Entry e = functions(y,x)->getEntry(k);
                    matrix(e.y,e.x) = e.val;
                }
			}
        }
    }
	matrix = MatrixXd::Zero(rows,columns);
}


void NMFMatrix::read(double* data){

	Map<MatrixXd> mapper(data,matrix.rows(),matrix.cols());
	matrix = mapper;
	data += matrix.size();

	for(int y = 0; y < matrix.rows(); ++y){
		for(int x = 0; x < matrix.cols(); ++x){
			functions(y,x)->fromDoubles(data);
			data += functions(y,x)->dataSize();
		}
	}
}

bool NMFMatrix::isConstrained(int row){
	for(int x = 0; x < matrix.cols(); ++x){
		if(functions(row,x)->size() > 1)
			return true;
	}
	return false;
}

void NMFMatrix::write(double* data){
	memcpy(data,matrix.data(),(matrix.size()*sizeof(double)));
	data += matrix.size();


	for(int y = 0; y < matrix.rows(); ++y){
		for(int x = 0; x < matrix.cols(); ++x){
			functions(y,x)->toDoubles(data);
			data += functions(y,x)->dataSize();
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


void NMFMatrix::resize(int newRows, int newCols){
	for(int i =0; i < rows; ++i){
		for(int j =0; j < columns; ++j){
			delete functions(i,j);
		}
	}
	matrix = MatrixXd::Zero(newRows,newCols);

	rows = newRows;
	columns = newCols;

	functions = MatrixXp(newRows,newCols);
	for(int i =0; i < rows; ++i){
		for(int j =0; j < columns; ++j){
			functions(i,j) = new PiecewisePF();
		}
	}

}
