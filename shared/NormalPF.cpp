//UniformPF.cpp
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/24/2017

#include "NormalPF.h"

NormalPF::NormalPF(int x, int y,double u,double sd){
	Entry ent = {x,y,0};
	value.push_back(ent);
	dist = normal_distribution<double>(u,sd);
}

vector<Entry> NormalPF::random(){
	value[0].val = dist(generator);
	return value;
}

void NormalPF::addObservation(double d){

}

string NormalPF::toString(){
	//more
	return "";
}
