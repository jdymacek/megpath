//Parallelized Function Thrower
//Matthew Dyer
//Created on 6/22/2017
//Last Modified: 6/22/2017

#ifndef PARALLELFUNCTHROW__H
#define PARALLELFUNCTHROW__H

#include "ParallelPatterns.h"
#include "FuncThrow.h"
#include "Distributed.h"

using namespace std;

class ParallelFuncThrow: public FuncThrow, public ParallelPatterns{
	public:
		ParallelFuncThrow();
		virtual void monteCallback(int iter);
//		virtual double monteCarlo();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
};


#endif
