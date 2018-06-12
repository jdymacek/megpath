//Parallelized Function Thrower
//Matthew Dyer
//Created on 6/22/2017
//Last Modified: 6/22/2017

#include"ParallelFuncThrow.h"

ParallelFuncThrow::ParallelFuncThrow(): ParallelPatterns(){
	program = "ParallelFuncThrow";
}

void ParallelFuncThrow::monteCallback(int iter){
	FuncThrow::monteCallback(iter);
}

bool ParallelFuncThrow::annealCallback(int iter){
	return ParallelPatterns::annealCallback(iter);
}

void ParallelFuncThrow::start(){
	ParallelPatterns::start();

    buffer = new double[FuncThrow::bufferSize];
    FuncThrow::recvBuffer = new double[FuncThrow::bufferSize];


}

void ParallelFuncThrow::run(){
	double error = 0;
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	finished();
	error = algorithm->anneal();
	//allAnnealAverage();
	//state->both = false;
	//error = algorithm->anneal();
	//allAnnealAverage();

	ParallelPatterns::gatherCoefficients();
}

void ParallelFuncThrow::stop(){
	delete[] buffer;
	delete[] FuncThrow::recvBuffer;
	ParallelPatterns::stop();
}
