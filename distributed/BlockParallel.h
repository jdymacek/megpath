//Block Parallel
//Justin Moore
//Created on : 6/3/2019
//Last Modified 6/7/2019

#ifndef BLOCKPARALLEL__H
#define BLOCKPARALLEL__H

#include "ParallelPatterns.h"

using namespace std;

struct BlockSet{
	MPI_Group group;
	MPI_Comm comm;
	Range subRange;
};

class BlockParallel: public ParallelPatterns{
	public:
		BlockParallel();
		virtual void start();	
		virtual void averageCoefficients();
		virtual void averagePatterns();
		virtual void groupAverage(NMFMatrix& mat, BlockSet set);
		virtual void run();
		virtual void gatherPatterns();
		virtual void stop();
		virtual void monteCallback(int iter);	
		virtual bool annealCallback(int iter);
		virtual void montePrintCallback(int iter);
		virtual void annealPrintCallback(int iter);
	protected:
		int rowUnique;
		int pCount;
		int pDisp;
		Range block;
		vector<BlockSet> rSets;
		vector<BlockSet> cSets;
};


#endif
