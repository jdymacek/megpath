//Parallelized Function Thrower
//Matthew Dyer
//Created on 6/22/2017
//Last Modified: 6/22/2017

#include"ParallelFuncThrow.h"

ParallelFuncThrow::ParallelFuncThrow(): Distributed(){
	ft = new FuncThrow();
	pp = new PatternMatching();
	program = "ParallelFuncThrow";
}

void ParallelFuncThrow::monteCallback(int iter){
	ft->monteCallback(iter);
}

bool ParallelFuncThrow::annealCallback(int iter){
	return pp->annealCallback(iter);
}

void ParallelFuncThrow::start(){
	Distributed::start();
	pp->state = state;
	pp->start();
	pp->algorithm = algorithm;
	ft->state = state;
	ft->start();
	ft->algorithm = algorithm;
}

void ParallelFuncThrow::run(){
	double error= 0;
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	ft->finished();
	pp->allAverage(state->patterns,MPI_COMM_WORLD);
	error = algorithm->anneal();
	pp->allAverage(state->patterns,MPI_COMM_WORLD);
	pp->gatherCoefficients();
}

void ParallelFuncThrow::stop(){
	Distributed::stop();
	string prev = state->stats;
	state->stats = "none";
	pp->stop();
	ft->stop();
	state->stats = prev;
}
