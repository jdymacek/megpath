//Threaded Analysis
//Dakota Martin
//Julian Dymacek
//Created on 5/23/2018

#include "Threaded.h"

Threaded::Threaded(int nt){
	numThreads = nt;
	program = "Threaded";
}

void Threaded::start(string filename){
	Analysis::start(filename);
	algorithm = new ThreadedMonteAnneal(state,numThreads);
}

void Threaded::run(){
	//Could put stop watch in here
	ProbFunc::generator.seed(time(0));
	algorithm->monteCarlo();
	algorithm->anneal();		
}

void Threaded::stop(){
	//state->patterns.write(state->analysis + "patterns.csv");
	//state->coefficients.write(state->analysis + "coefficients.csv");

	//ofstream fout;
	//fout.open(state->analysis + "expression.txt");
	//fout << state->coefficients.matrix*state->patterns.matrix;
	//fout.close();
}

