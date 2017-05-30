//ShiftPF.cpp -- Shifts a group of varibles
//julian dymacek
//Matthew Dyer
//created: 5/24/2017
//modified: 5/25/2017

#ifndef SHIFTPF__H
#define SHIFTPF__H

#include <vector>
#include <sstream>
#include "ProbFunc.h"
#include "HistoPF.h"

using namespace std;

class ShiftPF: public ProbFunc{
	public:
		ShiftPF();
		ShiftPF(vector<Entry> vec);
		double random();
		void addObservation(double d);
		int size();
		void setEntries(vector<Entry> vec);
		Entry getEntry(int i);
		string toString();
	private:
		HistoPF function;
		vector<Entry> org;
		vector<Entry> current;
};

#endif
