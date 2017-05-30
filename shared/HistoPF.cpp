//HistoPF -- Histogram based probabilty function
//Julian Dymacek
//Created 5/24/2017
//Modified  5/25/2017

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
	/*
		double prev;
		if((bin - 1) > 0){
			prev = intervals[bin-1];
		}else{
			prev = 0.0;
		}
	 */
	double mid = (current+next)/2;
	double diff = next - current;

	double howMuch = 0.0;
	if(v > mid){
		howMuch = next - v;
		weights[bin] += (howMuch+diff/2)*(weights.size());
		weights[bin+1] += (howMuch-diff/2)*(weights.size());
	}else if(v < mid){
		howMuch = v - current;
		weights[bin] += (howMuch+diff/2)*(weights.size());
		if((bin -1) > 0){
			weights[bin-1] += (diff-howMuch/2)*(weights.size());
		}else{
			weights[bin] += (diff-howMuch/2)*(weights.size());
		}
	}else{
		weights[bin] += 1;
	}

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

