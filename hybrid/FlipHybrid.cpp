//Hybrid -- combines PatternMatching with ThreadedMonteAnneal
//Julian Dymacek
//Dakota Martin
//Created on : 6/12/2018

#include "FlipHybrid.h"

FlipHybrid::FlipHybrid(int nt): PatternMatching(){
	program = "FlipHybrid";
	numThreads = nt;
}


void FlipHybrid::start(){
	PatternMatching::start();
	algorithm = new FlipThreadedMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

