//UniformPF.h
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/24/2017

#ifndef UNIFORM__PF
#define UNIFORM__PF

#include <iostream>
#include <vector>
#include "ProbFunc.h"

using namespace std;

class UniformPF: public ProbFunc{
	public:
		UniformPF(int x,int y);
		vector<Entry> random();
		void addObservation(double d);
		string toString();
	private:
		vector<Entry> value;
};
#endif
