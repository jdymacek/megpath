//UniformPF.cpp
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017

#include "UniformPF.h"

UniformPF::UniformPF(int x, int y){
	dist = uniform_real_distribution<double>(0.0,1.0);
}

double UniformPF::random(){
	return dist(generator);
}

string UniformPF::toString(){
<<<<<<< HEAD
	return ""; //TODO
=======
	return "uniform: [0,1)";
>>>>>>> f592d1b04813e454aac0ab0e65ab64c64b26b4e4
}
