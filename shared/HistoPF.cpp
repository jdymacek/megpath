//HistoPF -- Histogram based probabilty function
//Julian Dymacek
//Matthew Dyer
//Created 5/24/2017
//Modified on: 6/6/2017

#include "HistoPF.h"

HistoPF::HistoPF(){
	weights = vector<double>(22,1.0);
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
	double current = intervals[bin];
	double next = intervals[bin+1];
	double mid = (current+next)/2;
	double diff = next - current;

	double howMuch = abs(v-mid)/diff;
	if(v > mid){
		weights[bin] += 1 - howMuch;
		weights[bin+1] += howMuch;
	}else if(v < mid){
		weights[bin] += 1-howMuch;
		if((bin - 1) > 0){
			weights[bin-1] += howMuch;
		}
	}else{
		weights[bin] += 1;
	}

	dist = piecewise_linear_distribution<double>(intervals.begin(),intervals.end(),weights.begin());
}

string HistoPF::toString(){
	stringstream ss;
	for(int i =0; i < weights.size(); ++i){
		ss << weights[i];
		if(i != weights.size()-1){
			ss << ",";
		}
	}
	return ss.str();
}

vector<double> HistoPF::getVector(){
	return weights;
}

void HistoPF::setVector(vector<double> vec){
	weights = vec;
}
