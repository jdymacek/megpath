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
		void monteCarloThread(int xStart, int xEnd, int yStart, int yEnd,vector<int> core);
		void annealThread(int xStart, int xEnd,int yStart,int yEnd,vector<int> core);

		virtual double monteCarlo();
		virtual double anneal();
	protected:
		Barrier* barrier;
		int numThreads;
		thread::id rootId;
};

#endif
