//UniformPF.cpp
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/24/2017

#include "UniformPF.h"

UniformPF::UniformPF(int x, int y){
	Entry ent = {x,y,0};
	value.push_back(ent);
	dist = uniform_real_distribution<double>(0.0,1.0);
}

vector<Entry> UniformPF::random(){
	value[0].val = dist(generator);
	return value;
}

void UniformPF::addObservation(double d){

}

string UniformPF::toString(){
	return "";
}
