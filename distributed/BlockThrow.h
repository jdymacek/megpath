//Block Throw
//Justin Moore
//Created 6/7/2019
//Last Modified 6/7/2019

#ifndef BLOCKTHROW__H
#define BLOCKTHROW__H

#include "BlockParallel.h"
#include <algorithm>
using namespace std;

class BlockThrow: public BlockParallel{
	public:
		BlockThrow();
		virtual void start();	
//		virtual void averageCoefficients();
//		virtual void averagePatterns();
//		virtual void groupAverage(NMFMatrix& mat, BlockSet set);
		virtual void run();
//		virtual void gatherPatterns();
		virtual void stop();
		virtual void monteCallback(int iter);	
		virtual bool annealCallback(int iter);
		void throwPatterns();
		void throwPatterns2();
//		virtual void montePrintCallback(int iter);
//		virtual void annealPrintCallback(int iter);
	protected:
		BlockSet shareSet;
//		int rowUnique;
//		int pCount;
//		int pDisp;
//		Range block;
//		vector<BlockSet> rSets;
//		vector<BlockSet> cSets;
};


#endif
