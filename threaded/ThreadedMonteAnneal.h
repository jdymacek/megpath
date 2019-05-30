#ifndef THREADEDMONTEANNEAL__H
#define THREADEDMONTEANNEAL__H

#include <thread>
#include <vector>
#include "MonteAnneal.h"
#include "Barrier.h"


using namespace std;

//thinking that the global state should be declaired outside of class say here:

class ThreadedMonteAnneal:public MonteAnneal{
	public:
		ThreadedMonteAnneal(State* st, int nt);
		void monteCarloThread();
		void annealThread(double t, double alpha);

		virtual double monteCarlo();
		virtual double anneal();
	protected:
		map <thread::id, int> threadMap;
		Barrier* barrier;
		int numThreads;
		thread::id rootId;
};

#endif
