//FixedPF.cpp
//Julian Dymacek
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 5/25/2017

#include "FixedPF.h"

FixedPF::FixedPF(double v){
	value = v;
}

double FixedPF::random(){
	return value;
}

void FixedPF::addObservation(double v){
	value = v;
}

string FixedPF::toString(){
	stringstream ss;
	ss << "fixedpf: " << value;
	return ss.str();
}
