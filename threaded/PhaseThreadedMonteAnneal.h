#ifndef PHASETHREADEDMONTEANNEAL__H
#define PHASETHREADEDMONTEANNEAL__H

#include <thread>
#include <vector>
#include "MonteAnneal.h"
#include "Barrier.h"


using namespace std;

//thinking that the global state should be declaired outside of class say here:

class PhaseThreadedMonteAnneal:public MonteAnneal{
	public:
		PhaseThreadedMonteAnneal(State* st, int nt);
		void monteCarloThreadCoefficient();
		void monteCarloThreadPattern();
		void annealThreadCoefficient(double t, double alpha);
		void annealThreadPattern(double t, double alpha);

		virtual double monteCarlo();
		virtual double anneal();
	protected:
		map <thread::id, int> threadMap;
		State * dupe;
		Barrier* barrier;
		int numThreads;
		thread::id rootId;
};

#endif
