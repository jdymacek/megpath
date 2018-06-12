//Parallel Patterns
//Dakota Martin
//Created on : 6/12/2036

#ifndef FLIPHYBRID__H
#define FLIPHYBRID__H

#include "PatternMatching.h"
#include "FlipThreadedMonteAnneal.h"
#include <mpi.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

class FlipHybrid: public PatternMatching{
	public:
		FlipHybrid(int nt);
		virtual void start();
	protected:
		int numThreads;
};


#endif
