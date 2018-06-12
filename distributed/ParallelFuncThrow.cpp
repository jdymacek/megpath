//Parallelized Function Thrower
//Matthew Dyer
//Created on 6/22/2017
//Last Modified: 6/22/2017

#include"ParallelFuncThrow.h"

ParallelFuncThrow::ParallelFuncThrow(): Distributed(){
	program = "ParallelFuncThrow";
	ft = new FuncThrow();
	pp = new ParallelPatterns();
}

void ParallelFuncThrow::monteCallback(int iter){
	ft->monteCallback(iter);
}

bool ParallelFuncThrow::annealCallback(int iter){
	return pp->annealCallback(iter);
}

void ParallelFuncThrow::start(){
	pp->state = state;
	ft->state = state;
	ft->start();
}

void ParallelFuncThrow::run(){
	double error = 0;
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	ft->finished();
	ft->stop();
	pp->start();
	error = algorithm->anneal();
	pp->allAnnealAverage();
	pp->gatherCoefficients();
}

void ParallelFuncThrow::stop(){
	pp->stop();
}
