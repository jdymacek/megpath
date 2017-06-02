//HybridPF.h
//Matthew Dyer
//Created on 6/2/2017
//Last Modified: 6/2/2017

#ifndef HYBRIDPF__H
#define HYBRIDPF__H

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include "ProbFunc.h"

using namespace std;

class HybridPF: public ProbFunc{
	public:
		HybridPF();
		double random();
		void addObservation(double obs);	
		string toString();
	private:
		int n;
		normal_distribution<double> norm;
		uniform_real_distribution<double> uni;
};

#endif
