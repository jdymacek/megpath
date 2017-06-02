//NormalPF.h
//Julian Dymacek
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017

#ifndef NORMALPF__H
#define NORMALPF__H

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include "ProbFunc.h"

using namespace std;

class NormalPF: public ProbFunc{
	public:
		NormalPF(double mu,double sd);
		double random();
		void addObservation(double obs);	
		string toString();
	private:
		normal_distribution<double> dist;
		int total;
		int n;
		double squares;
		double mu;
		double sd;
};

#endif
