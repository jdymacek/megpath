#ifndef DISTNAIVE__H
#define DISTNAIVE__H

#include "Distributed.h"
#include <mpi.h>

using namespace std;

class DistNaive: public Distributed{
	public:
		DistNaive();
		bool annealCallback(int iter);
		virtual void run();
};


#endif
