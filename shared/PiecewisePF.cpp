//PiecewisePF -- Histogram based probabilty function
//Julian Dymacek
//Matthew Dyer
//Created 6/15/2017
//Modified on: 6/15/2017

#include "PiecewisePF.h"

PiecewisePF::PiecewisePF(){
	SIZE = 51;
	total = SIZE;
	weights = new double[SIZE];
	for(int i =0; i < SIZE; ++i){
		weights[i] = 1;
	}
	uniform = new UniformPF();
}

PiecewisePF::~PiecewisePF(){
	delete weights;
	delete uniform;
}

double PiecewisePF::random(){
	double r = uniform->random();
	double u = uniform->random();

	int bin =0;
	double running = weights[bin]/total;
	while(r > running){
		bin += 1;
		running += weights[bin]/total;
	}

	if(bin == 0)
		return 0;
	if(bin == SIZE-1)
		return 1;

	double s = ((double)(SIZE-1));
	double a = ((double)(bin-1))/s;
    double b = ((double)(bin+1))/s;

	double su = sqrt(u)/s;

	if(u < 0.5){
		return a + su;
	}
	return b - su;
}

void PiecewisePF::addObservation(double v){
	total += 1;

	double space = 1.0/((double)(SIZE-1));
	int bin = v*(SIZE-1);
	
	double b = (double)bin/((double)(SIZE-1));	
	double alpha = (v-b)/space;
	weights[bin] += 1-alpha;
	if(bin+1 < SIZE)
		weights[bin+1] += alpha;
}

string PiecewisePF::toString(){
	stringstream ss;
	for(int i =0; i < SIZE; ++i){
		ss << weights[i];
		if(i != SIZE-1){
			ss << ",";
		}
	}
	return ss.str();
}

void PiecewisePF::toDoubles(double* buffer){
	memcpy(buffer,weights,SIZE*sizeof(double));	
}

void PiecewisePF::fromDoubles(double* buffer){
	memcpy(weights,buffer,SIZE*sizeof(double));
	total = 0;
	for(int i =0; i < SIZE; ++i){
		total += weights[i];
	}
}

int PiecewisePF::dataSize(){
	return SIZE;
}

