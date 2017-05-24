//ShiftPF.cpp -- Shifts a group of varibles
//julian dymacek
//created: 5/24/2017
//modified: 5/24/2017

#ifndef SHIFTPF__H
#define SHIFTPF__H

#include <vector>
#include "ProbFunc.h"
#include "HistoPF.h"

using namespace std;

class ShiftPF: public ProbFunc{
	public:
		ShiftPF(vector<Entry> vec);
		vector<Entry> random();
		void addObservation(double d);
		string toString();
	private:
		HistoPF function;
		double prevRandom;
		vector<Entry> org;
		vector<Entry> value;
};

#endif
