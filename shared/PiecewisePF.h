//HistoPF.h -- Histogram Based Probability Function
//Julian Dymacek
//Matthew Dyer
//Created 5/24/2017
//Modified on: 6/6/2017
#ifndef PIECEWISEPF__H
#define PIECEWISEPF__H

#include <vector>
#include <random>
#include <sstream>
#include <string.h>
#include "UniformPF.h"
#include "ProbFunc.h"

using namespace std;

class PiecewisePF: public ProbFunc{
	public:
		PiecewisePF();
		~PiecewisePF();
		double random();
		void addObservation(double d);
		string toString();
		void toDoubles(double* buffer);
		void fromDoubles(double* buffer);
		int dataSize();	
		void reset();
	private:
		double* weights;
		double total;
		int SIZE;
		UniformPF* uniform;
};

#endif
