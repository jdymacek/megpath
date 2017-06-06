#ifndef MONTEANNEAL__H
#define MONTEANNEAL__H

#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>
#include "NMFMatrix.h"
#include "Stopwatch.h"
#include "UniformPF.h"

using namespace std;
class MonteAnneal: Analysis{
	public:
		MonteAnneal();
		virtual bool accept(double de,double t);
		virtual void monteCarloStep(NMFMatrix& m);
		virtual void annealStep(NMFMatrix& m);
		virtual void monteCarlo();
		virtual void annealStep();
		virtual void run();
		virtual void stop();
		virtual void start();

};

#endif
