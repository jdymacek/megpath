#ifndef PHASETHREADED__H
#define PHASETHREADED__H

#include "Analysis.h"
#include "PhaseThreadedMonteAnneal.h"

using namespace std;

class PhaseThreaded:public Analysis{
	public:
		PhaseThreaded(int nt);
		virtual bool annealCallback(int iterations);
		virtual void start(string filename);
		virtual void run();
	protected:
		int numThreads;
};

#endif
