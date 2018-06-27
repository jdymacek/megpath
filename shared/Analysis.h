//created by Julian Dymacek
//last modified: 5/23/2018

#ifndef ANALYSIS__H
#define ANALYSIS__H

#include "State.h"
#include "Stopwatch.h"
#include "MonteAnneal.h"
#include "Observer.h"

using namespace std;

class Analysis: public Observer{
	public:
		Analysis();
		virtual ~Analysis();
		virtual void monteStartCallback();
		virtual void annealStartCallback();
		virtual void annealPrintCallback(int iterations);
        	virtual void montePrintCallback(int iterations);
        	virtual bool annealCallback(int iterations);
        	virtual void monteCallback(int iterations);
		virtual void monteFinalCallback();
		virtual void annealFinalCallback();
		virtual void load(string filename);
		virtual void setState(State* st);
		virtual void start();
		virtual void run();
		virtual void stop();
		virtual void output();
		virtual void outputAll();
		virtual void outputStats();
		virtual void outputRuns();
		virtual void timedRun(int runTime);
		void setAlgorithm(MonteAnneal* al);
		void setName(string name);
		string ttime;
		Stopwatch watch;
		Stopwatch lap;
		State* state;
		MonteAnneal* algorithm;
	protected:
		string program;
		double cachedError;
};

#endif
