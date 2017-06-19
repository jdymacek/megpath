//Analysis.cpp 
//Julian Dymacek
//Created: 6/6/2017
//A base class for different ConNMF algorithms


#include "Analysis.h"

Analysis::Analysis(){
	state = NULL;
}

void Analysis::start(string filename){
	//initialize global variables
	state = new State();
	state->load(filename);
}

void Analysis::run(){

}

void Analysis::stop(){

}

void Analysis::output(){

}

