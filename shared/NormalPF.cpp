//NormalPF.cpp
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017

#include "NormalPF.h"

NormalPF::NormalPF(double mu,double sd){
	dist = normal_distribution<double>(mu,sd);
}

double NormalPF::random(){
	double rando = dist(generator);
	while(rando < 0.0 || rando > 1.0){
		rando = dist(generator);
	}
	return rando;
}

void NormalPF::addObservation(double obs){
	total += obs;
	++n;
	squares += obs + obs;
	mu = total/n;
	sd = squares + (n * mu * mu - 2 * total * mu);
	sd = sd/(n-1);
	sd = sqrt(sd);
	dist = normal_distribution<double>(mu,sd);
}

string NormalPF::toString(){
	string rv = "";
	stringstream ss;
	ss << "Mean: " << mu << " Std Dev: " << sd << endl;
	ss >> rv;
	return rv;
}
