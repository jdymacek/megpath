//created by Julian Dymacek
//last modified: 5/23/2018

#ifndef ANALYSIS__H
#define ANALYSIS__H

#include "State.h"
#include "MonteAnneal.h"
#include "Observer.h"

using namespace std;

class Analysis: public Observer{
	public:
		Analysis();
		virtual void annealPrintCallback(int iterations);
        	virtual void montePrintCallback(int iterations);
        	virtual bool annealCallback(int iterations);
        	virtual void monteCallback(int iterations);
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
		virtual void output();
		virtual void outputAll();
		virtual void outputStats();
		State* state;
		MonteAnneal* algorithm;
	protected:
		string program;
		double cachedError;
};

#endif
