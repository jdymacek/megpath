//Threaded Analysis
//Dakota Martin
//Julian Dymacek
//Created on 6/7/2018

#include "OpenMP.h"

OpenMP::OpenMP(int nt){
	numThreads = nt;
	program = "OpenMP";
}

bool OpenMP::annealCallback(int iterations){
	return true;
}

void OpenMP::start(string filename){
	Analysis::start(filename);
	algorithm = new OMPMonteAnneal(state,numThreads);
	program += "_" + to_string(numThreads) + "t";
}

void OpenMP::run(){
	//Could put stop watch in here
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	algorithm->anneal();
}


