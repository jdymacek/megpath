//ShiftPF.cpp -- Shifts a group of varibles
//julian dymacek
//created: 5/24/2017
//modified: 5/25/2017

#include "ShiftPF.h"

ShiftPF::ShiftPF(vector<Entry> vec){
	org = vec;
	current = vec;
}


double ShiftPF::random(){
	double r = function.random();
	for(int i =0; i < org.size(); ++i){
		current[i].val = org[i].val + r;
	}
	return r;
}

string ShiftPF::toString(){
	stringstream ss;
	ss << "shiftpf: [";
	for(int i =0; i < org.size(); ++i){
		ss << current[i].val;
		if(i < current.size()-1)
			ss << ",";
	}	
	ss << "]";
	//should print val
	return ss.str();
}

void ShiftPF::addObservation(double d){
	function.addObservation(d);	
}

int ShiftPF::size(){
	return org.size();
}

Entry ShiftPF::getEntry(int i){
	Entry e;
	if(i < 0 || i > org.size())
		return e;
	return current[i];
}
