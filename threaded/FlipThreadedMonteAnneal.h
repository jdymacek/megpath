#ifndef FLIPTHREADEDMONTEANNEAL__H
#define FLIPTHREADEDMONTEANNEAL__H

#include <thread>
#include <vector>
#include "MonteAnneal.h"
#include "Barrier.h"


using namespace std;


class FlipThreadedMonteAnneal:public MonteAnneal{
	public:
		FlipThreadedMonteAnneal(State* st, int nt);
		void monteCarloThreadCoefficient();
		void monteCarloThreadPattern();
		void annealThreadCoefficient(int num, double t, double alpha);
		void annealThreadPattern();

		virtual double monteCarlo();
		virtual double anneal();
	protected:
		map <thread::id, int> threadMap;
		vector<vector<int>> ranges;
		State * dupe;
		Barrier* barrier;
		int numThreads;
		thread::id rootId;
};

#endif
