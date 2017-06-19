//Parallel Patterns
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/12/2017

#ifndef PARALLELPATTERNS__H
#define PARALLELPATTERNS__H

#include "Distributed.h"
#include <mpi.h>
#include <unistd.h>

using namespace std;

class ParallelPatterns: public Distributed{
	public:
		ParallelPatterns();	
		virtual double monteCarlo();
		virtual double anneal();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
		int findStart(int myRank, int curSize, int numRows);
		int findRows(int myRank, int curSize, int numRows);
		double annealAgain();
	protected:
		MatrixXd oexpression;
		int startPoint;
};


#endif
