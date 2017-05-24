//NormalPF.h
//Julian Dymacek
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/24/2017

#ifndef NORMALPF__H
#define NORMALPF__H

#include <iostream>
#include <vector>
#include "ProbFunc.h"

using namespace std;

class NormalPF: public ProbFunc{
	public:
		NormalPF(int x,int y,double u,double sd);
		vector<Entry> random();
		void addObservation(double d);
		string toString();
	private:
		normal_distribution<double> dist;
		vector<Entry> value;
};

#endif
