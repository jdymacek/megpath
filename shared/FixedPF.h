#ifndef FIXEDPF__H
#define FIXEDPF__H

#include <vector>
#include "ProbFunc.h"

using namespace std;

class FixedPF: public ProbFunc{
	public:
		FixedPF(int x,int y, double v);
		vector<Entry> random();
		void addObservation(double d);
		string toString();
	private:
		vector<Entry> value;
};

#endif
