//Threaded Analysis
//Dakota Martin
//Julian Dymacek
//Created on 5/23/2018

#include "Threaded.h"

Threaded::Threaded(int nt){
	numThreads = nt;
	program = "Threaded";
}

void Threaded::montePrintCallback(int iterations){
	Analysis::montePrintCallback(iterations);
	cout << "Average Error: " << state->errorAvg/state->errorCount << endl;
	state->errorAvg = 0;
	state->errorCount = 0;
}

void Threaded::annealPrintCallback(int iterations){
	Analysis::annealPrintCallback(iterations); 
   cout << "Average Error: " << state->errorAvg/state->errorCount << endl;
    state->errorAvg = 0;
    state->errorCount = 0;
}


bool Threaded::annealCallback(int iterations){
	return true;
}

void Threaded::start(){
	Analysis::start();
	algorithm = new ThreadedMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

void Threaded::run(){
	//Could put stop watch in here
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	algorithm->anneal();
}


