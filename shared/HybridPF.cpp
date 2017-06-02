//HybridPF.h
//Matthew Dyer
//Created on 6/2/2017
//Last Modified: 6/2/2017

#include "HybridPF.h"

HybridPF::HybridPF(){
}

double HybridPF::random(){
	if(n < 1000){
		++n;
		return unif.random();
	}
	return norm.random();
}

void HybridPF::addObservation(double obs){
	norm.addObservation(obs);
}

string HybridPF::toString(){
	return "TODO";
}
