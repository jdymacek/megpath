#ifndef MONTEANNEAL__H
#define MONTEANNEAL__H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdio>
#include "State.h"
#include "Stopwatch.h"
#include "ProbFunc.h"
#include "UniformPF.h"
#include "ErrorFunction.h"
#include "ErrorFunctionRow.h"
#include "ErrorFunctionCol.h"
#include "FileUtil.h"
#include "Observer.h"
#include "../../Eigen/Core"

using namespace std;
class MonteAnneal{
	public:
		MonteAnneal(State* st);
		virtual bool accept(double de,double t);
		virtual void monteCarloStep(NMFMatrix& m,ErrorFunction* ef);
		virtual void monteCarloStep(NMFMatrix& m,ErrorFunction* ef, Range r);
		virtual void annealStep(NMFMatrix& m,double t, ErrorFunction* ef);
		virtual void annealStep(NMFMatrix& m,double t, ErrorFunction* ef, Range r);	
		virtual double monteCarlo();
		virtual double anneal();
		virtual void setObserver(Observer* obs);
		string program;
	protected:
		State* state;
		UniformPF* uniform;
		Observer* callback;
};

#endif
