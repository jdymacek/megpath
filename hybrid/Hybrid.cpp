//Hybrid -- combines PatternMatching with ThreadedMonteAnneal
//Julian Dymacek
//Dakota Martin
//Created on : 5/29/2018

#include "Hybrid.h"

Hybrid::Hybrid(int nt): PatternMatching(){
	program = "Hybrid";
	numThreads = nt;
}


void Hybrid::start(string filename){
	PatternMatching::start(filename);
	algorithm = new ThreadedMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

