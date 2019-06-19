//ProbFunc.h
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 5/25/2017

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
		virtual ~ProbFunc();
		virtual double random();
		virtual int dataSize();
		virtual void addObservation(double d){ };
		virtual string toString();
		virtual int size();
		virtual Entry getEntry(int i);
		virtual vector<double> getVector();
		virtual void setVector(vector<double> vec);
		virtual void toDoubles(double* buffer);
		virtual void fromDoubles(double* buffer);
		virtual void reset();
		virtual ProbFunc* copy();
		static mt19937_64 generator;
};


#endif
