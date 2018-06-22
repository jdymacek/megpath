#ifndef THREEMONTEANNEAL__H
#define THREEMONTEANNEAL__H

#include "MonteAnneal.h"
#include "ThreeErrorFunction.h"
#include "ThreeState.h"

using namespace std;
class ThreeMonteAnneal: public MonteAnneal{
	public:
		ThreeMonteAnneal(ThreeState* st);
		virtual double monteCarlo();
		virtual double anneal();
	protected:
		ThreeState * state;
};

#endif
