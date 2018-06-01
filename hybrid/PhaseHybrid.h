//Parallel Patterns
//Dakota Martin
//Created on : 5/29/2018

#ifndef PHASEHYBRID__H
#define PHASEHYBRID__H

#include "PatternMatching.h"
#include "PhaseThreadedMonteAnneal.h"
#include <mpi.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

class PhaseHybrid: public PatternMatching{
	public:
		PhaseHybrid(int nt);
		virtual void start(string filename);
	protected:
		int numThreads;
};


#endif
