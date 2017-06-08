#ifndef MONTEANNEAL__H
#define MONTEANNEAL__H

#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>
#include "Analysis.h"
#include "NMFMatrix.h"
#include "Stopwatch.h"
#include "ProbFunc.h"
#include "UniformPF.h"
#include "ErrorFunction.h"
#include "ErrorFunctionRow.h"
#include "ErrorFunctionCol.h"

using namespace std;
class MonteAnneal:public Analysis{
	public:
		MonteAnneal();
		virtual bool accept(double de,double t);
		virtual void monteCarloStep(NMFMatrix& m,ErrorFunction* ef);
		virtual void annealStep(NMFMatrix& m,double t, ErrorFunction* ef);
		virtual void monteCarlo();
		virtual void anneal();
		virtual void run();
		virtual void stop();
	protected:
		UniformPF* uniform;

};

#endif
