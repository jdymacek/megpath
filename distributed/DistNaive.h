#ifndef DISTNAIVE__H
#define DISTNAIVE__H

#include "Distributed.h"
#include <mpi.h>

using namespace std;

class DistNaive: public Distributed{
	public:
		DistNaive();
		bool annealCallback(int iter);
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
};


#endif
