//created by Julian Dymacek
//last modified: 5/23/2018

#ifndef ANALYSIS__H
#define ANALYSIS__H

#include "State.h"
#include "MonteAnneal.h"

using namespace std;

class Analysis{
	public:
		Analysis();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
		virtual void output();
		State* state;
		MonteAnneal* algorithm;
	protected:
		string program;
};

#endif
