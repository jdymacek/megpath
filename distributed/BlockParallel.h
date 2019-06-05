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

struct BlockSet{
	MPI_Group group;
	MPI_Comm comm;
	Range subRange;
};

class BlockParallel: public ParallelPatterns{
	public:
		BlockParallel();
//		void startSplit();
//		virtual void allAverage();
		virtual void start();	
		virtual void run();
		virtual void gatherPatterns();
		virtual void stop();
//		virtual void gatherCoefficients();
//		virtual void monteCallback(int iter);	
//		virtual bool annealCallback(int iter);
//		virtual void montePrintCallback(int iter);
//		virtual void annealPrintCallback(int iter);
//		virtual void monteFinalCallback();
//		virtual void annealFinalCallback();
	protected:
		int rowUnique;
		int pCount;
		int pDisp;
		Range block;
		vector<BlockSet> rSets;
		vector<BlockSet> cSets;
};


#endif
