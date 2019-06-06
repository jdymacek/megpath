#ifndef MONTEDEBUG__H
#define MONTEDEBUG__H

#include "MonteAnneal.h"

using namespace std;
class MonteDebug: virtual public MonteAnneal{
	public:
		MonteDebug(State* st);
		virtual double monteCarlo();
		virtual double anneal();
};

#endif
