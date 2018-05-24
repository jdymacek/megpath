//Parallel Patterns
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/12/2017

#ifndef PARALLELPATTERNS__H
#define PARALLELPATTERNS__H

#include "Distributed.h"
#include <mpi.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

class ParallelPatterns: virtual public Distributed, public Observer{
	public:
		ParallelPatterns();
		virtual void allAnnealAverage();
		virtual void allAverage();	
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
		virtual void gatherCoefficients();
		int findStart(int myRank, int curSize, int numRows);
		int findRows(int myRank, int curSize, int numRows);
	protected:
		int bufferSize;
	        double* sendBuffer;
		double* recvBuffer;

		MatrixXd oexpression;
		int startPoint;
};


#endif
