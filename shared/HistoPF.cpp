//HistoPF -- Histogram based probabilty function
//Julian Dymacek
//Created 5/24/2017
//Modified  5/25/2017

#include "HistoPF.h"

HistoPF::HistoPF(){
	weights = vector<double>(40,1.0);
	for(int i =0; i < weights.size(); ++i){
		intervals.push_back(i*1.0/(weights.size()-1));
	}	
	dist = piecewise_linear_distribution<double>(intervals.begin(),intervals.end(),weights.begin());
}

double HistoPF::random(){
	return dist(generator);
}

void HistoPF::addObservation(double v){
	int bin = ((weights.size()-1)*v);
	weights[bin] += 1;
    dist = piecewise_linear_distribution<double>(intervals.begin(),intervals.end(),weights.begin());
}

string HistoPF::toString(){
	stringstream ss;
	ss << "histopf [";
	for(int i =0; i < weights.size(); ++i){
		ss << weights[i];
		if(i != weights.size()-1){
			ss << ",";
		}
	}
	ss << "]";
	return ss.str();
}

