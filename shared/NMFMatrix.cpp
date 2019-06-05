//NMFMatrix.cpp
//Matthew Dyer
//Created on 6/1/2017
//Last Modified: 6/12/2017

#include "NMFMatrix.h"

NMFMatrix::NMFMatrix(){
	calculateSize();
}

NMFMatrix::~NMFMatrix(){
	for(int i =0; i < rows; ++i){
		for(int j =0; j < columns; ++j){
			delete functions(i,j);
		}
	}
	if(sendBuffer != NULL){
		delete sendBuffer;
	}
	if(recvBuffer != NULL){
		delete recvBuffer;
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

void NMFMatrix::calculateSize(){
	int rv = 0;
	for(int y =0; y < matrix.rows(); ++y){
		for(int x = 0; x < matrix.cols(); ++x){
			rv += functions(y,x)->dataSize();		
		}
	}
	bufferedSize = matrix.size()+rv;
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
	calculateSize();
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

void NMFMatrix::read(Range r, double* data){
	Map<MatrixXd> mapper(data,r.rowSize(),r.colSize());
	matrix.block(r.rowStart,r.colStart,r.rowSize(),r.colSize()) = mapper;
	data += r.rowSize()*r.colSize();
	
	for(int y = r.rowStart; y <= r.rowEnd; ++y){
		for(int x = r.colStart; x <= r.colEnd; ++x){
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

void NMFMatrix::write(Range r, double* data){
	MatrixXd temp(r.rowSize(),r.colSize());
	temp = matrix.block(r.rowStart,r.colStart,r.rowSize(),r.colSize());
	memcpy(data,temp.data(),((r.rowSize()*r.colSize())*sizeof(double)));
	data += r.rowSize()*r.colSize();

	for(int y = r.rowStart; y <= r.rowEnd; ++y){
		for(int x = r.colStart; x <= r.colEnd; ++x){
			functions(y,x)->toDoubles(data);
			data += functions(y,x)->dataSize();
		}
	}	
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
	calculateSize();
}

void NMFMatrix::createBuffers(){
	sendBuffer = new double[size()];
	recvBuffer = new double[size()];
}
