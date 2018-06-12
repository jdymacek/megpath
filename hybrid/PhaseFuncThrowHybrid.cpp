//Hybrid -- combines PatternMatching with ThreadedMonteAnneal
//Julian Dymacek
//Dakota Martin
//Created on : 6/12/2018

#include "PhaseFuncThrowHybrid.h"

PhaseFuncThrowHybrid::PhaseFuncThrowHybrid(int nt): ParallelFuncThrow(){
	program = "PhaseFuncThrowHybrid";
	numThreads = nt;
}


void PhaseFuncThrowHybrid::start(){
	ParallelFuncThrow::start();
	algorithm = new FlipThreadedMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

