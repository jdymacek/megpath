//NMFMatrix.h
//Matthew Dyer
//Created on 6/1/2017
//Last Modified: 6/12/2017

#ifndef NMFMATRIX__H
#define NMFMATRIX__H

#include <iostream>
#include <fstream>
#include "../../Eigen/Core"
#include "ProbFunc.h"
#include "PiecewisePF.h"
#include "PointerSupport.h"

using namespace std;
using namespace Eigen;

typedef Matrix<ProbFunc*, Dynamic, Dynamic> MatrixXp;

class NMFMatrix{
	public:
		NMFMatrix();
		~NMFMatrix();
		NMFMatrix(int rowss, int cols);
		void read(double* data);
		void write(double* data);
		void write(string filename);
		int size();
		void resize(int newRows, int newCols);


		int rows;
		int columns;
		MatrixXd matrix;
		MatrixXp functions;
};

#endif
