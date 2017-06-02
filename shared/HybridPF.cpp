//HybridPF.cpp
//Matthew Dyer
//Created on 6/2/2017
//Last Modified: 6/2/2017

#include "HybridPF.h"

HybridPF::HybridPF(int samps){
	samples = samps;
}

double HybridPF::random(){
	if(n < samples){
		++n;
		return unif.random();
	}
	return norm.random();
}

void HybridPF::addObservation(double obs){
	norm.addObservation(obs);
}

string HybridPF::toString(){
	if(n < samples){
		return unif.toString();
	}
	return norm.toString();
}
