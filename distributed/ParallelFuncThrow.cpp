//Parallelized Function Thrower
//Matthew Dyer
//Created on 6/22/2017
//Last Modified: 6/22/2017

#include"ParallelFuncThrow.h"

ParallelFuncThrow::ParallelFuncThrow(): Distributed(){
	program = "ParallelFuncThrow";
}

double ParallelFuncThrow::monteCarlo(){
	FuncThrow::monteCarlo();
}

void ParallelFuncThrow::start(string filename){
	ParallelPatterns::start(filename);
	bufferSize = state->patterns.size()+1;
	buffer = new double[bufferSize];
	recvBuffer = new double[bufferSize];
	srand(time(0));
}

void ParallelFuncThrow::run(){
	double error = 0;
	monteCarlo();
	error = FuncThrow::anneal();
	ParallelPatterns::gatherCoefficients();
}

void ParallelFuncThrow::stop(){
	delete[] buffer;
	delete[] recvBuffer;
	ParallelPatterns::stop();
}
