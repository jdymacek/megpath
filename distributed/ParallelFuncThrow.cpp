//Parallelized Function Thrower
//Matthew Dyer
//Created on 6/22/2017
//Last Modified: 6/22/2017

#include"ParallelFuncThrow.h"

ParallelFuncThrow::ParallelFuncThrow(): Distributed(){
	program = "ParallelFuncThrow";
}

void ParallelFuncThrow::monteCallback(int iter){
	FuncThrow::monteCallback(iter);
}

void ParallelFuncThrow::start(string filename){
	ParallelPatterns::start(filename);
	buffer = new double[ParallelPatterns::bufferSize];
}

void ParallelFuncThrow::run(){
	double error = 0;
	algorithm->monteCarlo();
	error = algorithm->anneal();
	ParallelPatterns::gatherCoefficients();
}

void ParallelFuncThrow::stop(){
	delete[] buffer;
	ParallelPatterns::stop();
}
