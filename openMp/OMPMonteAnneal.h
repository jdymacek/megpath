#ifndef OMPMONTEANNEAL__H
#define OMPMONTEANNEAL__H

#include <thread>
#include <vector>
#include "MonteAnneal.h"
#include <omp.h>

using namespace std;

//thinking that the global state should be declaired outside of class say here:

class OMPMonteAnneal:public MonteAnneal{
	public:
		OMPMonteAnneal(State* st, int nt);
		void monteCarloThread(int xStart, int xEnd, int yStart, int yEnd);
		void annealThread(int xStart, int xEnd,int yStart,int yEnd);

		virtual double monteCarlo();
		virtual double anneal();
	protected:
		int numThreads;
};

#endif
