//Hybrid -- combines PatternMatching with ThreadedMonteAnneal
//Julian Dymacek
//Dakota Martin
//Created on : 5/29/2018

#include "PhaseHybrid.h"

PhaseHybrid::PhaseHybrid(int nt): PatternMatching(){
	program = "PhaseHybrid";
	numThreads = nt;
}


void PhaseHybrid::start(){
	PatternMatching::start();
	algorithm = new PhaseThreadedMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

