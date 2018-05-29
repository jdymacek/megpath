//Parallel Patterns
//Dakota Martin
//Created on : 5/29/2018

#ifndef HYBRID__H
#define HYBRID__H

#include "PatternMatching.h"
#include "ThreadedMonteAnneal.h"
#include <mpi.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

class Hybrid: public PatternMatching{
	public:
		Hybrid();
		virtual void start(string filename);
	protected:
		int numThreads;
};


#endif
