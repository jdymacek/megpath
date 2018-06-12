//Parallel Patterns
//Dakota Martin
//Created on : 6/12/2018

#ifndef PHASEFUNCTHROWHYBRID__H
#define PHASEFUNCTHROWHYBRID__H

#include "ParallelFuncThrow.h"
#include "FlipThreadedMonteAnneal.h"
#include <mpi.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

class PhaseFuncThrowHybrid: public ParallelFuncThrow{
	public:
		PhaseFuncThrowHybrid(int nt);
		virtual void start();
	protected:
		int numThreads;
};


#endif
