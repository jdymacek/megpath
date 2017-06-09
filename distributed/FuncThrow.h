#ifndef DISTNAIVE__H
#define DISTNAIVE__H

#include "MonteAnneal.h"
#include <mpi.h>
#include <unistd.h>

using namespace std;

class FuncThrow: public MonteAnneal{
	public:
		FuncThrow();	
		virtual void monteCarlo();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
	protected:
		int rank;
		string hostname;
		int process;

};


#endif
