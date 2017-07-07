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

class PatternMatching: virtual public Distributed, public ParallelPatterns{
	public:
		PatternMatching();	
		virtual double monteCarlo();
		virtual double anneal(bool both);
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
		virtual void gatherCoefficients();
		int findStart(int myRank, int curSize, int numRows);
		int findRows(int myRank, int curSize, int numRows);
	protected:
		MatrixXd oexpression;
		int startPoint;
};


#endif
