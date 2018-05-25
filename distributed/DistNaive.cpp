/*
	Distributed main file
	Matthew Dyer
	Created on 5/31/2017
	Last Modified: 6/5/2017
 */

#include "DistNaive.h"

using namespace std;

DistNaive::DistNaive():Distributed(){
	program = "DistNaive";
}

void DistNaive::start(string filename){
	Distributed::start(filename);
}

bool DistNaive::annealCallback(int iter){
	return true;
}

void DistNaive::run(){
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	double formerError = algorithm->anneal();
	sendLeastError(0, formerError);
	cout << "\033[1;31m" << hostname << " " << rank << "\033[0m";
	cout <<  " heading out" << endl;
}

void DistNaive::stop(){
	Distributed::stop();
}
