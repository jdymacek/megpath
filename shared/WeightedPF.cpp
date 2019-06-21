//NormalPF.cpp
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017

#include "WeightedPF.h"

WeightedPF::WeightedPF(){
	mean = 0.5;
	variance = 0.01;
	alpha = 0.001;
	dist = normal_distribution<double>(0.5,0.1);
}

WeightedPF::WeightedPF(double mu,double sd){
	mean = mu;
	variance = sd*sd;
	alpha = 0.001;
	dist = normal_distribution<double>(mean,sd);
}

ProbFunc* WeightedPF::copy(){
	return new WeightedPF(mean,sqrt(variance));
}

double WeightedPF::random(){
	double rando = dist(generator);
	while(rando < 0.0 || rando > 1.0){
		rando = dist(generator);
	}
	return rando;
}

void WeightedPF::addObservation(double obs){
	double diff = obs-mean;
	double incr = alpha*diff;
	mean = mean + incr;
	variance = (1-alpha)*(variance+diff*incr);
	dist = normal_distribution<double>(mean,sqrt(variance));
}

string WeightedPF::toString(){
	string rv = "";
	stringstream ss;
	cout << mean << endl;
	ss << "Mean: " << mean << " Std Dev: " << sqrt(variance) << endl;
	return ss.str();
}

void WeightedPF::toDoubles(double* buffer){
	buffer[0] = mean;
	buffer[1] = variance;
}

void WeightedPF::fromDoubles(double* buffer){
	mean = buffer[0];
	variance = buffer[1];
	dist = normal_distribution<double>(mean,sqrt(variance));
}

void WeightedPF::average(double* buffer, double alpha = 0.5){
	mean = buffer[0]*alpha + mean*(1-alpha);
    dist = normal_distribution<double>(mean,sqrt(variance));
}


int WeightedPF::dataSize(){
	return 2;
}
