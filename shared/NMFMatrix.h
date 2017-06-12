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
#include "HistoPF.h"

using namespace std;
using namespace Eigen;

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

		MatrixXd matrix;
		ProbFunc*** functions;
		int rows;
		int columns;	
};

#endif
