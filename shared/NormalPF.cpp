//NormalPF.cpp
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017

#include "NormalPF.h"

NormalPF::NormalPF(double u,double sd){
	dist = normal_distribution<double>(u,sd);
}

double NormalPF::random(){
	return dist(generator);
}

string NormalPF::toString(){
	return "NORMAL PF -- TODO print mean and sd";
}
