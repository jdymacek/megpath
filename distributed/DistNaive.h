#ifndef DISTNAIVE__H
#define DISTNAIVE__H

#include "MonteAnneal.h"

using namespace std;

class DistNaive: public MonteAnneal{
	public:
		DistNaive();
		virtual run(string filename);
		virtual run();
		virtual stop();

	protected:
		int rank;

};


#endif
