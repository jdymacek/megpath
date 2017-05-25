//FixedPF.cpp
//Julian Dymacek
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 5/25/2017

#include "FixedPF.h"

FixedPF::FixedPF(int x,int y, double v){
	value = v;
}

double FixedPF::random(){
	return value;
}

string FixedPF::toString(){
	return "";//TODO
}
