//NormalPF.h
//Julian Dymacek
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017

#ifndef NORMALPF__H
#define NORMALPF__H

#include <iostream>
#include <vector>
#include "ProbFunc.h"

using namespace std;

class NormalPF: public ProbFunc{
	public:
		NormalPF(double u,double sd);
		double random();
		string toString();
	private:
		normal_distribution<double> dist;
};

#endif
