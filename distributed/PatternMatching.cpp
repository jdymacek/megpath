//Parallel Pattern Matching cpp file
//Matthew Dyer
//Created on : 7/7/2017
//Last Modified 7/7/2017

#include "PatternMatching.h"

PatternMatching::PatternMatching(): Distributed(){
	program = "PatternMatching";
}

void PatternMatching::start(string filename){
	ParallelPatterns::start();
}

double PatternMatching::monteCarlo(){
	
}

double PatternMatching::anneal(bool both){

}

void PatternMatching::gatherCoefficients(){
	ParallelPatterns::gatherCoefficients();
}

void PatternMatching::run(){
	ParallelPatterns::run();
}

void PatternMatching::stop(){
	Distributed::stop();	
}

