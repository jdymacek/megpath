//HistoPF -- Histogram based probabilty function
//Julian Dymacek
//Created 5/24/2017
//Modified  5/24/2017

#include "HistoPF.h"

HistoPF::HistoPF(int x,int y){
	Entry e = {x,y,0};
	value.push_back(e);

	//kludge

}

vector<Entry> HistoPF::random(){
	return value;
}

void HistoPF::addObservation(double v){

}

string HistoPF::toString(){
	return "";
}

