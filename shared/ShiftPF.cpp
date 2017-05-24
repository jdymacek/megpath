//ShiftPF.cpp -- Shifts a group of varibles
//julian dymacek
//created: 5/24/2017
//modified: 5/24/2017

#include "ShiftPF.h"

ShiftPF::ShiftPF(vector<Entry> vec){
	for(int i =0; i < vec.size(); ++i){
		org.push_back(vec[i]);
		value.push_back(vec[i]);
	}	
	function = HistoPF(0,0);
}


vector<Entry> ShiftPF::random(){
	vector<Entry> r  = function.random();
	prevRandom = r[0].val;
	for(int i =0; i < org.size(); ++i){
		value[i].val = org[i].val + prevRandom;
	}
	return value;
}

string ShiftPF::toString(){
	return "";
}

void ShiftPF::addObservation(double d){
	function.addObservation(prevRandom);	
}
