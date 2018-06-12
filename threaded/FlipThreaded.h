#ifndef FLIPTHREADED__H
#define PHASETHREADED__H

#include "Analysis.h"
#include "FlipThreadedMonteAnneal.h"

using namespace std;

class FlipThreaded:public Analysis{
	public:
		FlipThreaded(int nt);
		virtual bool annealCallback(int iterations);
		virtual void start();
		virtual void run();
	protected:
		int numThreads;
};

#endif
