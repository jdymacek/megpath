//created by Julian Dymacek
//last modified: 5/23/2018

#ifndef THREEANALYSIS__H
#define THREEANALYSIS__H

#include "ThreeErrorFunction.h"
#include "Analysis.h"
#include "ThreeState.h"
#include "Stopwatch.h"
#include "MonteAnneal.h"
#include "Observer.h"

using namespace std;

class ThreeAnalysis: public Analysis{
	public:
		ThreeAnalysis();
		virtual void annealFinalCallback();
        virtual void monteFinalCallback();
		virtual void annealPrintCallback(int iterations);
        virtual void montePrintCallback(int iterations);
        virtual bool annealCallback(int iterations);
		virtual void outputAll();
		virtual void outputStats();
};

#endif
