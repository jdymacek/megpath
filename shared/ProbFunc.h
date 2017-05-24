//ProbFunc.h
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 5/23/2017

#ifndef PROBFUNC__H
#define PROBFUNC__H

#include <iostream>
#include <vector>
#include <random>

using namespace std;

struct Entry{
	int x;
	int y;
	double val;
};

class ProbFunc{
	public:
		virtual vector<Entry> random() = 0;
		virtual void addObservation(double d) = 0;
		virtual string toString() = 0;
		static default_random_engine generator;
};

#endif
