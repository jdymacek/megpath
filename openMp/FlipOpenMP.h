#ifndef FLIPOPENMP__H
#define FLIPOPENMP__H

#include "Analysis.h"
#include "FlipOMPMonteAnneal.h"

using namespace std;

//thinking that the global state should be declaired outside of class say here:

class FlipOpenMP:public Analysis{
	public:
		FlipOpenMP(int nt);
		virtual void annealPrintCallback(int iterations);
		virtual bool annealCallback(int iterations);
		virtual void start();
		virtual void run();
	protected:
		int numThreads;
};

#endif
