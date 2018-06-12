#ifndef FLIPOMPMONTEANNEAL__H
#define FLIPOMPMONTEANNEAL__H

#include <thread>
#include <vector>
#include "MonteAnneal.h"
#include <omp.h>


using namespace std;


class FlipOMPMonteAnneal:public MonteAnneal{
	public:
		FlipOMPMonteAnneal(State* st, int nt);
		void monteCarloThreadCoefficient();
		void monteCarloThreadPattern();
		void annealThreadCoefficient(int num, double t, double alpha);
		void annealThreadPattern();

		virtual double monteCarlo();
		virtual double anneal();
	protected:
		vector<vector<int>> ranges;
		State * dupe;
		int numThreads;
};

#endif
