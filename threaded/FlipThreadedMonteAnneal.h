#ifndef FLIPTHREADEDMONTEANNEAL__H
#define FLIPTHREADEDMONTEANNEAL__H

#include <thread>
#include <vector>
#include "MonteAnneal.h"
#include "Barrier.h"


using namespace std;


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
		map <int, int> threadMap;
		vector<vector<int>> ranges;
		State * dupe;
		Barrier* barrier;
		int numThreads;
		thread::id rootId;
};

#endif
