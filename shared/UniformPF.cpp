//UniformPF.cpp
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017

#include "UniformPF.h"

UniformPF::UniformPF(){
	dist = uniform_real_distribution<double>(0.0,1.0);
}

double UniformPF::random(){
	return dist(generator);
}

string UniformPF::toString(){
	return "uniform: [0,1)";
}
