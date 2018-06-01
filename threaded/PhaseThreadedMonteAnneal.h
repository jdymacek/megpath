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
		void monteCarloThreadCoefficient(int Start, int End);
		void monteCarloThreadPattern();
		void annealThreadCoefficient(int Start, int End);
		void annealThreadPattern();

		virtual double monteCarlo();
		virtual double anneal();
	protected:
		State * dupe;
		Barrier* barrier;
		int numThreads;
		thread::id rootId;
};

#endif
