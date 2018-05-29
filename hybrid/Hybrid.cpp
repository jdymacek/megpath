//Parallel Patterns main file
//Julian Dymacek
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/13/2017

#include "Hybrid.h"

ParallelPatterns::ParallelPatterns(): PatternMatching(){
	program = "ParallelPatterns";
}


void ParallelPatterns::start(string filename){
	PatternMatching::start(filename);
	algorithm = new ThreadeMonteAnneal(state):
}

