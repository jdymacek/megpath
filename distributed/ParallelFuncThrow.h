//Parallelized Function Thrower
//Matthew Dyer
//Created on 6/22/2017
//Last Modified: 6/22/2017

#ifndef FUNCTHROW__H
#define FUNCTHROW__H

#include "ParalellPatterns.h"
#include "FuncThrow.h"

using namespace std;

class ParallelFuncThrow: public FuncThrow , public ParallelPatterns{
	public:
		ParallelFuncThrow();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
};


#endif
