#ifndef PARALLELPATTERNS__H
#define PARALLELPATTERNS__H

#include "MonteAnneal.h"
#include <mpi.h>
#include <unistd.h>

using namespace std;

class ParallelPatterns: public MonteAnneal{
	public:
		ParallelPatterns();	
		virtual double monteCarlo();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
	protected:
		int rank;
		string hostname;
		int process;

};


#endif
