//Parallel Pattern Matching
//Matthew Dyer
//Created on : 7/7/2017
//Last Modified 7/7/2017

#ifndef PATTERNMATCHING__H
#define PATTERNMATCHING__H

#include "ParallelPatterns.h"
#include "Distributed.h"
#include <mpi.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

class PatternMatching:  public ParallelPatterns{
	public:
		PatternMatching();
		void matchPatterns();
		void readMatrix(double* data, MatrixXd& matrix);
        void writeMatrix(double* data, MatrixXd& matrix);
		virtual bool annealCallback(int iter);
		virtual void monteCallback(int iter);
};


#endif
