//HistoPF.h -- Histogram Based Probability Function
//Julian Dymacek
//Created 5/24/2017
//Modified 5/24/2017
#ifndef HISTOPF__H
#define HISTOPF__H

#include <vector>
#include <random>
#include "ProbFunc.h"

using namespace std;

class HistoPF: public ProbFunc{
	public:
		HistoPF(int x,int y);
		vector<Entry> random();
		void addObservation(double d);
		string toString();
	private:
		piecewise_linear_distribution<double> dist;
		vector<double> intervals;
		vector<double> weights;
		vector<Entry> value;
};

#endif
