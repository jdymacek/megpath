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

void DistNaive::run(){
	algorithm->monteCarlo();
	double formerError = algorithm->anneal();
	sendLeastError(0, formerError);

}

void DistNaive::stop(){
	Distributed::stop();
}
