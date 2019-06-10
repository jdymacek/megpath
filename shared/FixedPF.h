//FixedPF.h
//Julian Dymacek
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 5/25/2017

#ifndef FIXEDPF__H
#define FIXEDPF__H

#include <vector>
#include "ProbFunc.h"
#include <sstream>

using namespace std;

class FixedPF: public ProbFunc{
	public:
		FixedPF(double v);
		double random();
		void addObservation(double v);
		string toString();
	private:
		double value;
};

#endif
