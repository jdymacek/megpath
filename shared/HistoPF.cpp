//HistoPF -- Histogram based probabilty function
//Julian Dymacek
//Created 5/24/2017
//Modified  5/24/2017

#include "HistoPF.h"
#include <sstream>

HistoPF::HistoPF(int x,int y){
	Entry e = {x,y,0};
	weights(12,1);
	for(int i =0; i < weights.size(); ++i){
		intervals.push_back(i*1.0/(weights.size()-1));
	}	
	distribution(intervals.begin(),intervals.end(),weights.begin());
}

vector<Entry> HistoPF::random(){
	e.val = distribution(generator);
	return value;
}

void HistoPF::addObservation(double v){
	int bin = ((weights.size()-1)*v);
	weights[bin] += 1;
}

string HistoPF::toString(){
	stringstream ss;
	ss << "[";
	for(int i =0; i < weights.size(); ++i){
		ss << weights[i];
		if(i != weights.size()-1){
			ss << ",";
		}
	}
	ss << "]";
	return ss.str();
}

