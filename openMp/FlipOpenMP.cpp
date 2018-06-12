//Threaded Analysis
//Dakota Martin
//Julian Dymacek
//Created on 6/11/2018

#include "FlipOpenMP.h"

FlipOpenMP::FlipOpenMP(int nt){
	numThreads = nt;
	program = "FlipOpenMP";
}

void FlipOpenMP::annealPrintCallback(int iterations){
	Analysis::annealPrintCallback(iterations);
	cout << "accepts: " << state->errorCount << endl;
	state->errorCount = 0;
}

bool FlipOpenMP::annealCallback(int iterations){
	if(state->both && iterations >= state->MAX_RUNS*state->annealCutOff)
		state->both = false;
	return true;
}

void FlipOpenMP::start(){
	Analysis::start();
	algorithm = new FlipOMPMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

void FlipOpenMP::run(){
	//Could put stop watch in here
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	algorithm->anneal();
}


