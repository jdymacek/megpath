#ifndef THREADED__H
#define THREADED__H

#include <unistd.h>
#include <string>
#include <thread>
#include <vector>
#include "MonteAnneal.h"
#include "Barrier.h"


using namespace std;

//thinking that the global state should be declaired outside of class say here:

class Threaded:public MonteAnneal{
	public:
		Threaded(int nt);
		void monteCarloThread(int xStart, int xEnd, int yStart, int yEnd);
		virtual double monteCarlo();
		virtual double anneal();
		virtual void run();
		virtual void stop();
	protected:
		Barrier* barrier;
		int numThreads;

};

#endif
