//UniformPF.h
//Matthew Dyer
//Julian Dymacek
//Created on 5/24/2017
//Last Modified: 5/25/2017

#ifndef UNIFORM__PF
#define UNIFORM__PF

#include <iostream>
#include <vector>
#include "ProbFunc.h"

using namespace std;

class UniformPF: public ProbFunc{
	public:
		UniformPF();
		double random();
		string toString();
	private:
		uniform_real_distribution<double> dist;
};

#endif
