//Threaded Analysis
//Dakota Martin
//Julian Dymacek
//Created on 6/5/2018

#include "FlipThreaded.h"

FlipThreaded::FlipThreaded(int nt){
	numThreads = nt;
	program = "FlipThreaded";
}

bool FlipThreaded::annealCallback(int iterations){
	return true;
}

void FlipThreaded::start(string filename){
	Analysis::start(filename);
	algorithm = new FlipThreadedMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

void FlipThreaded::run(){
	//Could put stop watch in here
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	algorithm->anneal();		
}


