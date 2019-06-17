//NormalPF.h
//Julian Dymacek
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017

#ifndef WEIGHTEDPF__H
#define WEIGTHEDPF__H

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include "ProbFunc.h"

using namespace std;

class WeightedPF: public ProbFunc{
	public:
		WeightedPF();
		WeightedPF(double mu,double sd);
		double random();
		void addObservation(double obs);	
		string toString();
		void toDoubles(double* buffer);
		void fromDoubles(double* buffer);
		int dataSize();
	private:
		normal_distribution<double> dist;
		double mean;
		double alpha;
		double variance;
};

#endif
