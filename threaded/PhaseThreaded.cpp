//Threaded Analysis
//Dakota Martin
//Julian Dymacek
//Created on 6/1/2018

#include "PhaseThreaded.h"

PhaseThreaded::PhaseThreaded(int nt){
	numThreads = nt;
	program = "PhaseThreaded";
}

bool PhaseThreaded::annealCallback(int iterations){
	return true;
}

void PhaseThreaded::start(string filename){
	Analysis::start(filename);
	algorithm = new PhaseThreadedMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

void PhaseThreaded::run(){
	//Could put stop watch in here
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	algorithm->anneal();		
}


