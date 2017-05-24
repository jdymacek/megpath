#ifndef HISTOPF__H
#define HISTOPF__H

#include <vector>
#include "ProbFunc.h"

using namespace std;

struct Atom{
	double center;
	int count;
	int prev;
	int next;
};

class HistoPF: public ProbFunc{
	public:
		HistoPF(int x,int y);
		vector<Entry> random();
		void addObservation(double d);
		string toString();
	private:
		vector<Atom> bins;
		int totalObservations;
		vector<Entry> value;
};

#endif
