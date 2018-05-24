//Standard Analysis functions
//Julian Dymacek
//Dakota Martin
//Created on 5/23/2018
//Last Modified: 5/23/2018

#include "Standard.h"

Standard::Standard(){
	program = "MonteAnneal";
}

void Standard::monteCallback(double error){
}

void Standard::annealCallback(double error){
}


void Standard::run(){
	//Could put stop watch in here
	ProbFunc::generator.seed(time(0));
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	algorithm->anneal();		
}

void Standard::stop(){
	//state->patterns.write(state->analysis + "patterns.csv");
	//state->coefficients.write(state->analysis + "coefficients.csv");

	//ofstream fout;
	//fout.open(state->analysis + "expression.txt");
	//fout << state->coefficients.matrix*state->patterns.matrix;
	//fout.close();
}
