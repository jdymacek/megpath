#ifndef MONTEANNEAL__H
#define MONTEANNEAL__H

#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>
#include "NMFMatrix.h"
#include "Stopwatch.h"
#include "ProbFunc.h"
#include "ErrorFunctionRow.h"
#include "ErrorFunctionCol.h"

using namespace std;
class MonteAnneal:public Analysis{
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
	protected:
		UniformPF* uniform;

};

#endif
