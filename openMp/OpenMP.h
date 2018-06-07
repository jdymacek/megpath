#ifndef OPENMP__H
#define OPENMP__H

#include "Analysis.h"
#include "OMPMonteAnneal.h"

using namespace std;

//thinking that the global state should be declaired outside of class say here:

class OpenMP:public Analysis{
	public:
		OpenMP(int nt);
		virtual bool annealCallback(int iterations);
		virtual void start(string filename);
		virtual void run();
	protected:
		int numThreads;
};

#endif
