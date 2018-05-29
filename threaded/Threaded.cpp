//Threaded Analysis
//Dakota Martin
//Julian Dymacek
//Created on 5/23/2018

#include "Threaded.h"

Threaded::Threaded(int nt){
	numThreads = nt;
	program = "Threaded";
}

bool Threaded::annealCallback(int iterations){
	return true;
}

void Threaded::start(string filename){
	Analysis::start(filename);
	algorithm = new ThreadedMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

void Threaded::run(){
	//Could put stop watch in here
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	algorithm->anneal();		
}


