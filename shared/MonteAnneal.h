#ifndef MONTEANNEAL__H
#define MONTEANNEAL__H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdio>
#include "Analysis.h"
#include "NMFMatrix.h"
#include "Stopwatch.h"
#include "ProbFunc.h"
#include "UniformPF.h"
#include "ErrorFunction.h"
#include "ErrorFunctionRow.h"
#include "ErrorFunctionCol.h"
#include "FileUtil.h"
#include "../../Eigen/Core"

using namespace std;
class MonteAnneal:public Analysis{
	public:
		MonteAnneal();
		virtual bool accept(double de,double t);
		virtual void monteCarloStep(NMFMatrix& m,ErrorFunction* ef);
		virtual void annealStep(NMFMatrix& m,double t, ErrorFunction* ef);
		virtual double monteCarlo();
		virtual double anneal();
		virtual void run();
		virtual void stop();
		void output();
	protected:
		UniformPF* uniform;
};

#endif
