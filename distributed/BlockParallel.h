//Parallel Patterns
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/12/2017

#ifndef BLOCKPARALLEL__H
#define BLOCKPARALLEL__H

#include "ParallelPatterns.h"
#include <mpi.h>
#include <unistd.h>
#include <algorithm>
#include <vector>

using namespace std;

class BlockParallel: public ParallelPatterns{
	public:
		BlockParallel();
//		void startSplit();
//		virtual void allAverage();
		virtual void start();	
		virtual void run();
		virtual void stop();
//		virtual void gatherCoefficients();
//		virtual void monteCallback(int iter);	
//		virtual bool annealCallback(int iter);
//		virtual void montePrintCallback(int iter);
//		virtual void annealPrintCallback(int iter);
//		virtual void monteFinalCallback();
//		virtual void annealFinalCallback();
	protected:
		int count;
		int disp;
		int rowUnique;
		Range block;
		MatrixXd oexpression;
		vector<MPI_Group> rGrps;
		vector<MPI_Comm> rComms;
		vector<MPI_Group> cGrps;
		vector<MPI_Comm> cComms;
};


#endif
