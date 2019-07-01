//NMFMatrix.cpp
//Matthew Dyer
//Created on 6/1/2017
//Last Modified: 6/12/2017

#include "NMFMatrix.h"

NMFMatrix::NMFMatrix(){
	calculateSize();
	prototype = NULL;
	sendBuffer = NULL;
	recvBuffer = NULL;
}

NMFMatrix::~NMFMatrix(){
	for(int i =0; i < matrix.rows(); ++i){
		for(int j =0; j < matrix.cols(); ++j){
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

NMFMatrix::NMFMatrix(int rowss, int cols, ProbFunc* p = NULL){
	prototype = p;

	matrix = MatrixXd::Zero(matrix.rows(),matrix.cols());
	functions = MatrixXp(matrix.rows(),matrix.cols());

	for(int i =0; i < matrix.rows(); ++i){
		for(int j =0; j < matrix.cols(); ++j){
			functions(i,j) = prototype->copy();
		}
	}	
	calculateSize();
	sendBuffer = NULL;
	recvBuffer = NULL;
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

int NMFMatrix::size(Range r){
	int rv = 0;
	for(int y = r.rowStart; y <= r.rowEnd; ++y){
		for(int x = r.colStart; x <= r.colEnd; ++x){
			rv += functions(y,x)->dataSize();		
		}
	}
	return r.rowSize()*r.colSize()+rv;
}

void NMFMatrix::reset(){
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
	matrix = MatrixXd::Zero(matrix.rows(),matrix.cols());
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

int NMFMatrix::read(double* data, Range r){
	Map<MatrixXd> mapper(data,r.rowSize(),r.colSize());
	matrix.block(r.rowStart,r.colStart,r.rowSize(),r.colSize()) = mapper;
	data += r.rowSize()*r.colSize();
	int rv = r.rowSize()*r.colSize();

	for(int y = r.rowStart; y <= r.rowEnd; ++y){
		for(int x = r.colStart; x <= r.colEnd; ++x){
			functions(y,x)->fromDoubles(data);
			int s = functions(y,x)->dataSize();
			data += s;
			rv += s;
		}
	}
	return rv;
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

int NMFMatrix::write(double* data, Range r){
	MatrixXd temp(r.rowSize(),r.colSize());
	temp = matrix.block(r.rowStart,r.colStart,r.rowSize(),r.colSize());
	memcpy(data,temp.data(),(temp.size()*sizeof(double)));
	data += temp.size();
	int rv = temp.size();
	for(int y = r.rowStart; y <= r.rowEnd; ++y){
		for(int x = r.colStart; x <= r.colEnd; ++x){
			functions(y,x)->toDoubles(data);
			int s = functions(y,x)->dataSize();
			data += s;
			rv += s;
		}
	}
	return rv;
}

int NMFMatrix::average(double* data, Range r, double alpha = 0.5){
	int rr = write(recvBuffer,r);
	for(int i =0; i < rr; ++i){
		data[i] = data[i]*alpha + recvBuffer[i]*(1-alpha);
	}

    Map<MatrixXd> mapper(data,r.rowSize(),r.colSize());
	MatrixXd a = mapper*alpha;
	MatrixXd b = matrix.block(r.rowStart,r.colStart,r.rowSize(),r.colSize())*(1-alpha); 
	
    matrix.block(r.rowStart,r.colStart,r.rowSize(),r.colSize()) = a+b;

    data += r.rowSize()*r.colSize();
    int rv = r.rowSize()*r.colSize();

    for(int y = r.rowStart; y <= r.rowEnd; ++y){
        for(int x = r.colStart; x <= r.colEnd; ++x){
            functions(y,x)->average(data,alpha);
            int s = functions(y,x)->dataSize();
            data += s;
            rv += s;
        }
    }
    return rv;
}

void NMFMatrix::transition(){
	for(int i =0; i < matrix.rows(); ++i){
		for(int j =0; j < matrix.cols(); ++j){
			functions(i,j)->transition();
		}
	}
}

void NMFMatrix::resize(int newRows, int newCols){

	for(int i =0; i < matrix.rows(); ++i){
		for(int j =0; j < matrix.cols(); ++j){
			delete functions(i,j);
		}
	}
	matrix = MatrixXd::Zero(newRows,newCols);

	functions = MatrixXp(newRows,newCols);
	for(int i =0; i < matrix.rows(); ++i){
		for(int j =0; j < matrix.cols(); ++j){
			functions(i,j) = prototype->copy();
		}
	}
	calculateSize();
}

void NMFMatrix::shrink(Range block){
	MatrixXd temp2 = matrix.block(block.rowStart, block.colStart, block.rowSize(), block.colSize());
	matrix = temp2;
	for(int i =0; i < matrix.rows(); ++i){
		for(int j =0; j < matrix.cols(); ++j){
			if(!block.contains(i,j))
				delete functions(i,j);
		}
	}
	MatrixXp temp = functions.block(block.rowStart, block.colStart, block.rowSize(), block.colSize());
	functions = temp;
	calculateSize();
}

void NMFMatrix::fixRange(Range r){
	for(int i = r.rowStart; i <= r.rowEnd; i++){
		for(int j = r.colStart; j <= r.colEnd; j++){
			delete functions(i,j);
			functions(i,j) = new FixedPF(0.0);
		}
	}
}

void NMFMatrix::observeRange(Range r){
	for(int i = r.rowStart; i <= r.rowEnd; i++){
		for(int j = r.colStart; j <= r.colEnd; j++){
			functions(i,j)->addObservation(matrix(i,j));
		}
	}

}

void NMFMatrix::createBuffers(){
	if(sendBuffer != NULL){
		delete sendBuffer;
	}
	if(recvBuffer != NULL){
		delete recvBuffer;
	}
	sendBuffer = new double[size()];
	recvBuffer = new double[size()];
}
