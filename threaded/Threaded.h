#ifndef THREADED__H
#define THREADED__H

#include <unistd.h>
#include <string>
#include "MonteAnneal.h"



using namespace std;

//thinking that the global state should be declaired outside of class say here:

class Threaded:public MonteAnneal{
	public:
		MonteAnneal();
		virtual double monteCarlo();
		virtual double anneal();
		virtual void run();
		virtual void stop();
	protected:
};

#endif
