//NMFMatrix.h
//Matthew Dyer
//Created on 6/1/2017
//Last Modified: 6/1/2017

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
		void writeNMFMatrix(NMFMatrix& mat,string filename);
		void createNMFMatrix(NMFMatrix& rv,int rows,int columns,double (*functionPtr)(int, int));

		MatrixXd matrix;
		ProbFunc*** functions;
		int rows;
		int columns;	
		double (*errorFunction)(int,int);
};

#endif
