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
		virtual double monteCarlo();
		virtual double anneal();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
		virtual void gatherCoefficients();
		void readMatrix(double* data, MatrixXd& matrix);
        void writeMatrix(double* data, MatrixXd& matrix);
	protected:
		MatrixXd oexpression;
		int startPoint;
};


#endif
